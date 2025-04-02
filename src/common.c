/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * NovelKit
 * Copyright (c) 2025, Tamako Mori. All rights reserved.
 */

/*
 * common.h: Common routines.
 */

#include "novelkit.h"

bool load_file_content(const char *file, char **buf)
{
	struct file *f;
	size_t file_size, read_size;

	assert(buf != NULL);

	if (!file_open(file, &f)) {
		return false;
	}

	if (!file_get_size(f, &file_size))
		return false;

	*buf = malloc(file_size);
	if (*buf == NULL) {
		sys_out_of_memory();
		return false;
	}

	if (!file_read(f, *buf, file_size, &read_size)) {
		sys_error("Could not read file \"%s\".", file);
		free(buf);
		return false;
	}

	file_close(f);

	return true;
}
