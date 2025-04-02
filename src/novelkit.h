/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * Main header.
 */

#ifndef NOVELKIT_NOVELKIT_H
#define NOVELKIT_NOVELKIT_H

/* For compiler compatibility. */
#include "compat.h"

/* Linguine */
#include "linguine/linguine.h"

/* MediaKit */
#include "mediakit/mediakit.h"

/* Internals */
#include "api.h"
#include "common.h"
#include "scenario.h"

/* Standard C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Linguine runtime. */
extern struct rt_env *rt;

#endif
