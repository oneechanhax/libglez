/*
 * fonts.c
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#include "internal/fonts.h"

#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <assert.h>

internal_font_t loaded_fonts[GLEZ_FONT_COUNT];

texture_font_t *internal_font_get(glez_font_t handle)
{
    assert(handle < GLEZ_FONT_COUNT);
    assert(loaded_fonts[handle].init);

    return loaded_fonts[handle].font;
}

void internal_fonts_init()
{
    memset(loaded_fonts, 0, sizeof(loaded_fonts));
}

void internal_fonts_destroy()
{
    for (glez_font_t i = 0; i < GLEZ_FONT_COUNT; ++i)
    {
        if (loaded_fonts[i].init)
        {
            glez_font_unload(i);
        }
    }
}

glez_font_t glez_font_load(const char *path, float size)
{
    assert(path != NULL);
    assert(size > 0);

    internal_font_t result;
    memset(&result, 0, sizeof(result));

    result.atlas = texture_atlas_new(1024, 1024, 1);
    if (result.atlas == NULL)
        return GLEZ_FONT_INVALID;

    result.font = texture_font_new_from_file(result.atlas, size, path);
    if (result.font == NULL)
    {
        texture_atlas_delete(result.atlas);
        return GLEZ_FONT_INVALID;
    }

    result.font->hinting = 0;

    for (glez_font_t i = 0; i < GLEZ_FONT_COUNT; ++i)
    {
        if (loaded_fonts[i].init == 0)
        {
            result.init = 1;
            memcpy(&loaded_fonts[i], &result, sizeof(result));
            return i;
        }
    }

    return GLEZ_FONT_INVALID;
}

void glez_font_unload(glez_font_t handle)
{
    assert(handle < GLEZ_FONT_COUNT);
    assert(loaded_fonts[handle].init);

    texture_atlas_delete(loaded_fonts[handle].atlas);
    texture_font_delete(loaded_fonts[handle].font);

    loaded_fonts[handle].init = 0;
}

void glez_font_string_size(glez_font_t font, const char *string, float *out_x,
                           float *out_y)
{
    float pen_x = 0;

    float size_x = 0;
    float size_y = 0;

    texture_font_t *fnt = internal_font_get(font);
    if (fnt == NULL)
        return;
    texture_font_load_glyphs(fnt, string);

    for (size_t i = 0; i < strlen(string); ++i)
    {
        texture_glyph_t *glyph = texture_font_find_glyph(fnt, &string[i]);
        if (glyph == NULL)
        {
            continue;
        }

        pen_x += texture_glyph_get_kerning(glyph, &string[i]);
        pen_x += glyph->advance_x;
        if (pen_x > size_x)
            size_x = pen_x;

        if (glyph->height > size_y)
            size_y = glyph->height;
    }
    if (out_x)
        *out_x = size_x;
    if (out_y)
        *out_y = size_y;
}
