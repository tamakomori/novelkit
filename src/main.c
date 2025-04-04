/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

#include "novelkit.h"

/* The runtime. */
struct rt_env *rt;

/* Forward declaration. */
static bool load_novelkit_file(void);
static bool load_main_file(void);
static bool call_setup(char **title, int *width, int *height);
static void print_error(struct rt_env *rt);

/*
 * App initialization
 *  - This function is called when the app is going to be initialized.
 */
bool on_hal_init_render(char **title, int *width, int *height)
{
	/* Create a language runtime. */
	if (!rt_create(&rt))
		return false;

	/* Install the NovelKit API to the runtime. */
	if (!install_api(rt))
		return false;

	/* Load the "novelkit.ls" file. */
	if (!load_novelkit_file())
		return false;

	/* Load the "main.ls" file. */
	if (!load_main_file())
		return false;

	/* Call "setup()" and get a title and window size. */
	if (!call_setup(title, width, height))
		return false;

	/* Return a title, width, and height. */
	return true;
}

/* Load "novelkit.ls". */
static bool load_novelkit_file(void)
{
	char *buf;

	/* Load a file content, i.e., a script text. */
	if (!common_load_file_content("game.ls", &buf))
		return false;

	/* Register the script text to the language runtime. */
	if (!rt_register_source(rt, "game.ls", buf)) {
		print_error(rt);
		return false;
	}

	free(buf);

	return true;
}

/* Load "main.ls". */
static bool load_main_file(void)
{
	char *buf;

	/* Load a file content, i.e., a script text. */
	if (!common_load_file_content("main.ls", &buf))
		return false;

	/* Register the script text to the language runtime. */
	if (!rt_register_source(rt, "main.ls", buf)) {
		print_error(rt);
		return false;
	}

	free(buf);

	return true;
}

/* Call "setup()" function to determin a title, width, and height. */
static bool call_setup(char **title, int *width, int *height)
{
	struct rt_value ret;
	struct rt_value title_val;
	struct rt_value width_val;
	struct rt_value height_val;
	const char *title_s;

	if (!rt_call_with_name(rt, "setup", NULL, 0, NULL, &ret))
		return false;

	if (!rt_get_dict_elem(rt, &ret, "title", &title_val))
		return false;
	if (!rt_get_string(rt, &title_val, &title_s))
		return false;
	*title = strdup(title_s);

	if (!rt_get_dict_elem(rt, &ret, "width", &width_val))
		return false;
	if (!rt_get_int(rt, &width_val, width))
		return false;

	if (!rt_get_dict_elem(rt, &ret, "height", &height_val))
		return false;
	if (!rt_get_int(rt, &height_val, height))
		return false;

	return true;
}

/*
 * Rendering initialization.
 *  - This function is called right before the game loop.
 */
bool on_hal_ready(void)
{
	struct rt_value ret;

	/* Call the "first()" function to setup a game system. */
	if (!rt_call_with_name(rt, "first", NULL, 0, NULL, &ret)) {
		print_error(rt);
		return false;
	}

	return true;
}

/*
 * Frame handeler.
 *  - This function is called every frame periodically.
 */
bool on_hal_frame(void)
{
	/* Run a tag. */
	if (!scenario_run_tag(rt)) {
		print_error(rt);
		return false;
	}

	return true;
}

/*
 * Helpers
 */

/* Print an error message. */
static void print_error(struct rt_env *rt)
{
	sys_error(_("%s:%d: error: %s\n"),
		  rt_get_error_file(rt),
		  rt_get_error_line(rt),
		  rt_get_error_message(rt));
}
