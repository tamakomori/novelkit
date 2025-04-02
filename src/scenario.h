/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * scenario.h: Scenario management.
 */

#ifndef NOVELKIT_SCENARIO_H
#define NOVELKIT_SCENARIO_H

#include "compat.h"

bool init_scenario(void);
void cleanup_scenario(void);
bool move_to_scenario(const char *file);

#endif
