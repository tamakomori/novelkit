/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * scenario.h: Scenario management
 */

#ifndef NOVELKIT_SCENARIO_H
#define NOVELKIT_SCENARIO_H

#include "compat.h"

bool scenario_init(void);
void scenario_cleanup(void);
bool scenario_move_to_file(struct rt_env *rt, const char *file);
bool scenario_run_tag(struct rt_env *rt);

#endif
