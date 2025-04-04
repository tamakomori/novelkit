/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * API: The implementation of NovelKit API.
 */

#include "novelkit.h"

/* API error message. */
static char api_error_message[4096];

/* Forward declarations. */
static void copy_api_error(struct rt_env *rt);
static bool get_int_param(struct rt_env *rt, const char *name, int *ret);
static bool get_float_param(struct rt_env *rt, const char *name, float *ret);
static bool get_string_param(struct rt_env *rt, const char *name, const char **ret);

/*
 * NovelKit.moveToScenario()
 */
bool NovelKit_moveToScenario(struct rt_env *rt)
{
	const char *file;

	if (!get_string_param(rt, "file", &file))
		return false;

	if (!scenario_move_to_file(rt, file)) {
		copy_api_error(rt);
		return false;
	}

	return true;
}

/* Get an integer parameter. */
__attribute__((unused))
static bool get_int_param(struct rt_env *rt, const char *name, int *ret)
{
	struct rt_value param, elem;

	if (!rt_get_local(rt, "param", &param))
		return false;

	if (!rt_get_dict_elem(rt, &param, name, &elem))
		return false;

	switch (elem.type) {
	case RT_VALUE_INT:
		*ret = elem.val.i;
		break;
	case RT_VALUE_FLOAT:
		*ret = (int)elem.val.f;
		break;
	case RT_VALUE_STRING:
		*ret = atoi(elem.val.str->s);
		break;
	default:
		rt_error(rt, "Unexpected parameter value for %s.", name);
		return false;
	}

	return true;
}

/* Get a float parameter. */
__attribute__((unused))
static bool get_float_param(struct rt_env *rt, const char *name, float *ret)
{
	struct rt_value param, elem;

	if (!rt_get_local(rt, "param", &param))
		return false;

	if (!rt_get_dict_elem(rt, &param, name, &elem))
		return false;

	switch (elem.type) {
	case RT_VALUE_INT:
		*ret = (float)elem.val.i;
		break;
	case RT_VALUE_FLOAT:
		*ret = elem.val.f;
		break;
	case RT_VALUE_STRING:
		*ret = (float)atof(elem.val.str->s);
		break;
	default:
		rt_error(rt, "Unexpected parameter value for %s.", name);
		return false;
	}

	return true;
}

/* Get a string parameter. */
static bool get_string_param(struct rt_env *rt, const char *name, const char **ret)
{
	struct rt_value param, elem;
	static char buf[128];

	if (!rt_get_local(rt, "param", &param))
		return false;

	if (!rt_get_dict_elem(rt, &param, name, &elem))
		return false;

	switch (elem.type) {
	case RT_VALUE_INT:
		snprintf(buf, sizeof(buf), "%d", elem.val.i);
		*ret = buf;
		break;
	case RT_VALUE_FLOAT:
		snprintf(buf, sizeof(buf), "%f", elem.val.f);
		*ret = buf;
		break;
	case RT_VALUE_STRING:
		*ret = elem.val.str->s;
		break;
	default:
		rt_error(rt, "Unexpected parameter value for %s.", name);
		return false;
	}

	return true;
}

/*
 * Install API functions to a runtime.
 */
bool install_api(struct rt_env *rt)
{
	const char *params[] = {"param"};
	struct func {
		const char *name;
		const char *field;
		bool (*func)(struct rt_env *);
	} funcs[] = {
		{"NovelKit_moveToScenario", "moveToScenario", NovelKit_moveToScenario},
	};
	const int tbl_size = sizeof(funcs) / sizeof(struct func);
	struct rt_value dict;
	int i;

	/* Make a global variable "NovelKit". */
	if (!rt_make_empty_dict(rt, &dict))
		return false;
	if (!rt_set_global(rt, "NovelKit", &dict))
		return false;

	/* Register functions. */
	for (i = 0; i < tbl_size; i++) {
		struct rt_value funcval;

		/* Register a cfunc. */
		if (!rt_register_cfunc(rt, funcs[i].name, 1, params, funcs[i].func))
			return false;

		/* Get a function value. */
		if (!rt_get_global(rt, funcs[i].name, &funcval))
			return false;

		/* Make a dictionary element. */
		if (!rt_set_dict_elem(rt, &dict, funcs[i].field, &funcval))
			return false;
	}

	return true;
}

/*
 * Put an error log. (called from API implementation)
 */
void api_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vsnprintf(api_error_message, sizeof(api_error_message), format, ap);
	va_end(ap);
}

/* Put an out-of-memory log. (called from API implementation) */
void api_out_of_memory(void)
{
	api_error("Out of memory.");
}

/* Copy an API error message. */
static void copy_api_error(struct rt_env *rt)
{
	rt_error(rt, api_error_message);
}
