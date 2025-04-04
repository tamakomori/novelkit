/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * scenario.c: Scenario Management
 */

#include "novelkit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* False assertion */
#define NEVER_COME_HERE		0

/* Sizes. */
#define TAG_NAME_MAX	128
#define PROP_MAX	128
#define PROP_NAME_MAX	128
#define PROP_VALUE_MAX	4096
#define COMMAND_MAX	65536

/* Command struct. */
struct command {
	char *tag_name;
	int prop_count;
	char *prop_name[PROP_MAX];
	char *prop_value[PROP_MAX];
};

/* Current scenario file. */
__attribute__((unused))
static char *cur_file;

/* Current command index. */
__attribute__((unused))
static int cur_index;

/* Command table. */
__attribute__((unused))
static struct command cmd[COMMAND_MAX];

/* Command size. */
static int cmd_size;

/* Forward declaration. */
static void destroy_commands(void);
static void print_error(struct rt_env *rt);
static bool parse_tag_callback(const char *name, int props, const char **prop_name, const char **prop_val);
static bool parse_tag_document(const char *doc, bool (*callback)(const char *, int, const char **, const char **), char **error_msg, int *error_line);

/*
 * Initialize the scenario module.
 */
bool scenario_init(void)
{
	destroy_commands();

	return true;
}

/*
 * Cleanup the scenario module.
 */
void scenario_cleanup(void)
{
	destroy_commands();
}

static void destroy_commands(void)
{
	struct command *c;
	int i, j;

	cur_index = 0;

	if (cur_file != NULL) {
		free(cur_file);
		cur_file = NULL;
	}

	for (i = 0; i < cmd_size; i++) {
		c = &cmd[i];
		free(c->tag_name);
		for (j = 0; j < PROP_MAX; j++) {
			if (c->prop_name[i] != NULL) {
				free(c->prop_name[i]);
				c->prop_name[i] = NULL;
			}
			if (c->prop_value[i] != NULL) {
				free(c->prop_value[i]);
				c->prop_value[i] = NULL;
			}
		}
	}
}

/*
 * Load a scenario file and move to it.
 */
bool scenario_move_to_file(struct rt_env *rt, const char *file)
{
	char *buf;
	char *error_message;
	int error_line;

	UNUSED_PARAMETER(rt);

	if (!common_load_file_content(file, &buf))
		return false;

	destroy_commands();

	if (!parse_tag_document(buf, parse_tag_callback, &error_message, &error_line)) {
		api_error("tag error: %s:%d: %s", file, error_line, error_message);
		free(error_message);
		free(buf);
		return false;
	}

	free(buf);

	return true;
}

/* Callback for when a tag is read. */
static bool parse_tag_callback(const char *name, int props, const char **prop_name, const char **prop_value)
{
	struct command *c;
	int i;

	/* If command table is full. */
	if (cmd_size >= COMMAND_MAX) {
		api_error("Too many commands.");
		return false;
	}

	c = &cmd[cmd_size++];
	c->prop_count = props;

	/* Copy a tag name. */
	c->tag_name = strdup(name);
	if (c->tag_name == NULL) {
		api_out_of_memory();
		return false;
	}

	/* Copy properties. */
	for (i = 0; i < props; i++) {
		c->prop_name[i] = strdup(prop_name[i]);
		if (c->prop_name[i] == NULL) {
			api_out_of_memory();
			return false;
		}

		c->prop_value[i] = strdup(prop_value[i]);
		if (c->prop_value[i] == NULL) {
			api_out_of_memory();
			return false;
		}
	}

	return true;
}

/*
 * Run a tag.
 *  - cont ... set to true if a next tag is requied to be run continuously.
 */
bool scenario_run_tag(struct rt_env *rt)
{
	struct command *c;
	struct rt_value dict;
	struct rt_value str;
	struct rt_value ret;
	int i;
	bool succeeded;

	assert(cur_index < cmd_size);

	c = &cmd[cur_index];

	succeeded = false;
	do {
		/* Make a parameter dictionary. */
		if (!rt_make_empty_dict(rt, &dict))
			break;

		/* Setup properties as dictionary items. */
		for (i = 0; i < c->prop_count; i++) {
			if (!rt_make_string(rt, &str, c->prop_value[i]))
				break;
			if (!rt_set_dict_elem(rt, &dict, c->prop_name[i], &str))
				break;
		}

		/* Call the corresponding function. */
		if (!rt_call_with_name(rt, c->tag_name, NULL, 1, &dict, &ret))
			break;

		/* Ok. */
		succeeded = true;
	} while (0);

	/* If failed: */
	if (!succeeded) {
		print_error(rt);
		return false;
	}

	/* Ok. */
	return true;
}

/*
 * Helper
 */

/* Print an error message. */
static void print_error(struct rt_env *rt)
{
	sys_error(_("%s:%d: error: %s\n"),
		  rt_get_error_file(rt),
		  rt_get_error_line(rt),
		  rt_get_error_message(rt));
}

/*
 * Tag Document Parser
 */

/* State machine */
enum state {
	ST_INIT,
	ST_TAGNAME,
	ST_PROPNAME,
	ST_PROPVALUE_QUOTE,
	ST_PROPVALUE_BODY,
};

/* Parse a tag document. */
bool
parse_tag_document(
	const char *doc,
	bool (*callback)(const char *, int, const char **, const char **),
	char **error_msg,
	int *error_line)
{
	static char tag_name[TAG_NAME_MAX];
	static char prop_name[PROP_MAX][PROP_NAME_MAX];
	static char prop_val[PROP_MAX][PROP_VALUE_MAX];

	const char *top;
	char c;
	int state;
	int line;
	int len;
	int prop_count;
	char *prop_name_tbl[PROP_MAX];
	char *prop_val_tbl[PROP_MAX];
	int i;

	for (i = 0; i < PROP_MAX; i++) {
		prop_name_tbl[i] = &prop_name[i][0];
		prop_val_tbl[i] = &prop_val[i][0];
	}

	top = doc;
	line = 1;
	len = 0;
	prop_count = 0;
	while (*top != '\0') {
		c = *top++;
		switch (state) {
		case ST_INIT:
			if (c == '[') {
				state = ST_TAGNAME;
				len = 0;
				continue;
			}
			if (c == '\n') {
				line++;
				continue;
			}
			if (c == ' ' || c == '\r' || c == '\t')
				continue;

			*error_msg = strdup(_("Invalid character."));
			*error_line = line;
			return false;
		case ST_TAGNAME:
			if (len == 0 && (c == ' ' || c == '\r' || c == '\t' || c == '\n'))
				continue;
			if (c == '\n')
				line++;
			if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
				assert(len > 0);
				tag_name[len] = '\0';
				state = ST_PROPNAME;
				len = 0;
				continue;
			}
			if (c == ']') {
				tag_name[len] = '\0';
				if (!callback(tag_name, 0, NULL, NULL)) {
					*error_msg = strdup(_("Too many properties."));
					*error_line = line;
					return false;
				}
				state = ST_INIT;
				continue;
			}
			if (len >= TAG_NAME_MAX) {
				*error_msg = strdup(_("Tag name too long."));
				*error_line = line;
				return false;
			}
			tag_name[len++] = c;
			continue;
		case ST_PROPNAME:
			if (prop_count == PROP_MAX) {
				*error_msg = strdup(_("Too many properties."));
				*error_line = line;
				return false;
			}
			if (len == 0 && c == ' ')
				continue;
			if (len == 0 && c == ']') {
				if (!callback(tag_name, prop_count, (const char **)prop_name_tbl, (const char **)prop_val_tbl)) {
					*error_msg = strdup(_("Internal error."));
					*error_line = line;
					return false;
				}
				state = ST_INIT;
				continue;
			}
			if (len == 0 && c == '\n')
				line++;
			if (len == 0 && (c == ' ' || c == '\r' || c == '\t' || c == '\n'))
				continue;
			if (len > 0 && c == '=') {
				assert(len > 0);

				/* Terminate the property name. */
				prop_name[prop_count][len] = '\0';

				state = ST_PROPVALUE_QUOTE;
				len = 0;
				continue;
			}
			if (len >= PROP_NAME_MAX) {
				*error_msg = strdup(_("Property name too long."));
				*error_line = line;
				return false;
			}
			if ((c >= 'a' && c <= 'z') ||
			    (c >= 'A' && c <= 'Z') ||
			    (c >= '0' && c <= '9') ||
			    c == '-' ||
			    c == '_') {
				prop_name[prop_count][len++] = c;
				continue;
			}
			*error_msg = strdup(_("Invalid character."));
			*error_line = line;
			continue;
		case ST_PROPVALUE_QUOTE:
			if (c == '\n')
				line++;
			if (c == ' ' || c == '\r' || c == '\t' || c == '\n')
				continue;
			if (c == '\"') {
				state = ST_PROPVALUE_BODY;
				len = 0;
				continue;
			}
			continue;
		case ST_PROPVALUE_BODY:
			if (c == '\\') {
				switch (*top) {
				case '\"':
					prop_val[prop_count][len] = '\"';
					len++;
					top++;
					continue;
				case 'n':
					prop_val[prop_count][len] = '\n';
					len++;
					top++;
					continue;
				case '\\':
					prop_val[prop_count][len] = '\\';
					len++;
					top++;
					continue;
				default:
					prop_val[prop_count][len] = '\\';
					len++;
					continue;
				}
			}
			if (c == '\"') {
				prop_val[prop_count][len] = '\0';
				prop_count++;

				state = ST_PROPNAME;
				len = 0;
				continue;
			}
			if (len >= PROP_VALUE_MAX) {
				*error_msg = strdup(_("Property value too long."));
				*error_line = line;
				return false;
			}
			prop_val[prop_count][len++] = c;
			continue;
		default:
			assert(NEVER_COME_HERE);
			break;
		}
	}

	if (state == ST_INIT)
		return true;

	*error_msg = strdup(_("Unexpected EOF"));
	*error_line = line;
	return false;
}
