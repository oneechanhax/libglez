/*
 * textures.h
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include <GL/gl.h>

typedef struct internal_texture_s
{
    char bound;
    char init;

    int width;
    int height;

    GLuint texture_id;
    char filename[256];

    GLubyte *data;
} internal_texture_t;

void internal_textures_init();

void internal_textures_destroy();

int internal_texture_load_png_rgba(const char *name, internal_texture_t *out);

internal_texture_t *internal_texture_get(glez_texture_t handle);

void internal_texture_bind(glez_texture_t handle);
