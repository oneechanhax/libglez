/*
 * fonts.h
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "glez.h"

#include "freetype-gl.h"

typedef struct internal_font_s
{
    int init;

    texture_font_t *font;
    texture_atlas_t *atlas;
} internal_font_t;

texture_font_t *internal_font_get(glez_font_t handle);

void internal_fonts_init();

void internal_fonts_destroy();
