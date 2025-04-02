/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * API: The implementation of NovelKit API.
 */

#ifndef NOVELKIT_API_H
#define NOVELKIT_API_H

#include "compat.h"
#include "linguine/linguine.h"

/* Install API functions to a runtime. */
bool install_api(struct rt_env *rt);

/* Put an error log. (called from API implementation) */
void api_error(const char *format, ...);

/* Put an out-of-memory log. (called from API implementation) */
void api_out_of_memory(void);

/* Scenario API */
bool NovelKit_moveToScenarioFile(struct rt_env *rt);

#endif
