/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

#include "novelkit.h"

struct rt_env *rt;

static bool load_game_file(void);
static bool call_setup(char **title, int *width, int *height);
static void print_error(struct rt_env *rt);

/*
 * Screen initialization
 */
bool on_hal_init_render(char **title, int *width, int *height)
{
	if (!rt_create(&rt))
		return false;

	if (!install_api(rt))
		return false;

	if (!load_game_file())
		return false;

	if (!call_setup(title, width, height))
		return false;

	return true;
}

static bool load_game_file(void)
{
	struct file *f;
	size_t file_size, read_size;
	char *buf;

	if (!file_open("game.ls", &f))
		return false;

	if (!file_get_size(f, &file_size))
		return false;

	buf = malloc(file_size);
	if (buf == NULL) {
		sys_out_of_memory();
		return false;
	}

	if (!file_read(f, buf, file_size, &read_size))
		return false;

	file_close(f);

	if (!rt_register_source(rt, "game.ls", buf)) {
		print_error(rt);
		return false;
	}

	free(buf);

	return true;
}

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
 * Called after the whole HAL initialization and before the game loop.
 */
bool on_hal_ready(void)
{
	struct rt_value ret;

	if (!rt_call_with_name(rt, "first", NULL, 0, NULL, &ret)) {
		print_error(rt);
		return false;
	}

	return true;
}

/*
 * Called every frame.
 */
bool on_hal_frame(void)
{
	return true;
}

static void print_error(struct rt_env *rt)
{
	sys_error(_("%s:%d: error: %s\n"),
		  rt_get_error_file(rt),
		  rt_get_error_line(rt),
		  rt_get_error_message(rt));
}
