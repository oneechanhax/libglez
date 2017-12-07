/*
 * draw.h
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "glez.h"

#include <vec234.h>

struct vertex_main
{
    vec2 position;
    vec2 tex_coords;
    glez_rgba_t color;
    int mode;
};

struct draw_state
{
    char dirty;
    GLuint texture;
    glez_font_t font;
} ds;

void ds_init();

void ds_destroy();

void ds_pre_render();

void ds_post_render();

void ds_bind_texture(GLuint texture);
