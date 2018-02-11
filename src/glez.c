/*
 * glez.c
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#include "glez.h"

#include "internal/program.h"
#include "internal/draw.h"
#include "internal/fonts.h"
#include "internal/textures.h"

#include <math.h>

/* State functions */

void glez_init(int width, int height)
{
    ds_init();
    program_init(width, height);
    internal_fonts_init();
    internal_textures_init();
}

void glez_shutdown()
{
    ds_destroy();
    internal_fonts_destroy();
    internal_textures_destroy();
}

void glez_begin()
{
    ds_pre_render();
}

void glez_end()
{
    ds_post_render();
}

void glez_resize(int width, int height)
{
    shader_screen_size(width, height);
}

/* Drawing functions */

void glez_line(float x, float y, float dx, float dy, glez_rgba_t color,
               float thickness)
{
    /*x += 0.375f;
    y += 0.375f;*/

    GLuint indices[6] = { 0, 1, 3, 3, 2, 0 };

    struct vertex_main vertices[4];

    float nx = -dy;
    float ny = dx;

    float ex = x + dx;
    float ey = y + dy;

    float length = sqrtf(nx * nx + ny * ny);

    if (length == 0)
        return;

    length /= thickness;
    nx /= length;
    ny /= length;

    vertices[0].position.x = x - nx;
    vertices[0].position.y = y - ny;
    vertices[0].color      = color;
    vertices[0].mode       = DRAW_MODE_PLAIN;

    vertices[1].position.x = x + nx;
    vertices[1].position.y = y + ny;
    vertices[1].color      = color;
    vertices[1].mode       = DRAW_MODE_PLAIN;

    vertices[2].position.x = ex - nx;
    vertices[2].position.y = ey - ny;
    vertices[2].color      = color;
    vertices[2].mode       = DRAW_MODE_PLAIN;

    vertices[3].position.x = ex + nx;
    vertices[3].position.y = ey + ny;
    vertices[3].color      = color;
    vertices[3].mode       = DRAW_MODE_PLAIN;

    vertex_buffer_push_back(program.buffer, vertices, 4, indices, 6);
}

void glez_rect(float x, float y, float w, float h, glez_rgba_t color)
{
    /*x += 0.375f;
    y += 0.375f;*/

    struct vertex_main vertices[4];
    GLuint indices[6] = { 0, 1, 2, 2, 3, 0 };

    vertices[0].position.x = x;
    vertices[0].position.y = y;
    vertices[0].color      = color;
    vertices[0].mode       = DRAW_MODE_PLAIN;

    vertices[1].position.x = x;
    vertices[1].position.y = y + h;
    vertices[1].color      = color;
    vertices[1].mode       = DRAW_MODE_PLAIN;

    vertices[2].position.x = x + w;
    vertices[2].position.y = y + h;
    vertices[2].color      = color;
    vertices[2].mode       = DRAW_MODE_PLAIN;

    vertices[3].position.x = x + w;
    vertices[3].position.y = y;
    vertices[3].color      = color;
    vertices[3].mode       = DRAW_MODE_PLAIN;

    vertex_buffer_push_back(program.buffer, vertices, 4, indices, 6);
}

void glez_rect_outline(float x, float y, float w, float h, glez_rgba_t color,
                       float thickness)
{
    glez_line(x, y, w, 0, color, thickness);
    glez_line(x + w, y, 0, h, color, thickness);
    glez_line(x + w, y + h, -w, 0, color, thickness);
    glez_line(x, y + h, 0, -h, color, thickness);
}

void glez_rect_textured(float x, float y, float w, float h, glez_rgba_t color,
                        glez_texture_t texture, float tx, float ty, float tw,
                        float th)
{
    internal_texture_t *tex = internal_texture_get(texture);
    internal_texture_bind(texture);

    /*x += 0.375f;
    y += 0.375f;*/

    GLuint idx = program_next_index();

    struct vertex_main vertices[4];
    GLuint indices[6] = { idx, idx + 1, idx + 2, idx + 2, idx + 3, idx };

    float s0 = tx / tex->width;
    float s1 = (tx + tw) / tex->width;
    float t0 = ty / tex->height;
    float t1 = (ty + th) / tex->height;

    vertices[0].position.x   = x;
    vertices[0].position.y   = y;
    vertices[0].tex_coords.x = s0;
    vertices[0].tex_coords.y = t1;
    vertices[0].color        = color;
    vertices[0].mode         = DRAW_MODE_TEXTURED;

    vertices[1].position.x   = x;
    vertices[1].position.y   = y + h;
    vertices[1].tex_coords.x = s0;
    vertices[1].tex_coords.y = t0;
    vertices[1].color        = color;
    vertices[1].mode         = DRAW_MODE_TEXTURED;

    vertices[2].position.x   = x + w;
    vertices[2].position.y   = y + h;
    vertices[2].tex_coords.x = s1;
    vertices[2].tex_coords.y = t0;
    vertices[2].color        = color;
    vertices[2].mode         = DRAW_MODE_TEXTURED;

    vertices[3].position.x   = x + w;
    vertices[3].position.y   = y;
    vertices[3].tex_coords.x = s1;
    vertices[3].tex_coords.y = t1;
    vertices[3].color        = color;
    vertices[3].mode         = DRAW_MODE_TEXTURED;

    vertex_buffer_push_back(program.buffer, vertices, 4, indices, 6);
}

/* INTERNAL FUNCTION */
void draw_string_internal(float x, float y, const char *string,
                          texture_font_t *fnt, glez_vec4_t color, float *out_x,
                          float *out_y)
{
    float pen_x  = x;
    float pen_y  = y + fnt->height / 1.5f;
    float size_y = 0;

    if (fnt->atlas->id == 0)
    {
        glGenTextures(1, &fnt->atlas->id);
    }
    ds_bind_texture(fnt->atlas->id);
    if (fnt->atlas->dirty)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fnt->atlas->width,
                     fnt->atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE,
                     fnt->atlas->data);
        fnt->atlas->dirty = 0;
    }

    int len = strlen(string);
    if (len == 0)
        return;

    for (size_t i = 0; i < len; ++i)
    {
        texture_glyph_t *glyph = texture_font_find_glyph(fnt, &string[i]);
        if (glyph == NULL)
        {
            texture_font_load_glyph(fnt, &string[i]);
            continue;
        }
        GLuint indices[6];
        struct vertex_main vertices[4];
        if (i > 0)
        {
            x += texture_glyph_get_kerning(glyph, &string[i - 1]);
        }

        float x0 = (int)(pen_x + glyph->offset_x);
        float y0 = (int)(pen_y - glyph->offset_y);
        float x1 = (int)(x0 + glyph->width);
        float y1 = (int)(y0 + glyph->height);
        float s0 = glyph->s0;
        float t0 = glyph->t0;
        float s1 = glyph->s1;
        float t1 = glyph->t1;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 2;
        indices[4] = 3;
        indices[5] = 0;

        vertices[0] = (struct vertex_main){ (vec2){ x0, y0 }, (vec2){ s0, t0 },
                                            color, DRAW_MODE_FREETYPE };
        vertices[1] = (struct vertex_main){ (vec2){ x0, y1 }, (vec2){ s0, t1 },
                                            color, DRAW_MODE_FREETYPE };
        vertices[2] = (struct vertex_main){ (vec2){ x1, y1 }, (vec2){ s1, t1 },
                                            color, DRAW_MODE_FREETYPE };
        vertices[3] = (struct vertex_main){ (vec2){ x1, y0 }, (vec2){ s1, t0 },
                                            color, DRAW_MODE_FREETYPE };

        pen_x += glyph->advance_x;
        //pen_x = (int) pen_x + 1;
        if (glyph->height > size_y)
            size_y = glyph->height;

        vertex_buffer_push_back(program.buffer, vertices, 4, indices, 6);
    }

    if (out_x)
        *out_x = pen_x - x;
    if (out_y)
        *out_y = size_y;
}

void glez_string(float x, float y, const char *string, glez_font_t font,
                 glez_rgba_t color, float *out_x, float *out_y)
{
    texture_font_t *fnt = internal_font_get(font);

    ds_bind_texture(fnt->atlas->id);

    fnt->rendermode        = RENDER_NORMAL;
    fnt->outline_thickness = 0.0f;

    draw_string_internal(x, y, string, fnt, color, out_x, out_y);
}

void glez_string_with_outline(float x, float y, const char *string,
                              glez_font_t font, glez_rgba_t color,
                              glez_rgba_t outline_color, float outline_width,
                              int adjust_outline_alpha, float *out_x,
                              float *out_y)
{
    if (adjust_outline_alpha)
        outline_color.a = color.a;

    texture_font_t *fnt = internal_font_get(font);

    fnt->rendermode        = RENDER_OUTLINE_POSITIVE;
    fnt->outline_thickness = outline_width;
    draw_string_internal(x, y, string, fnt, outline_color, NULL, NULL);

    fnt->rendermode        = RENDER_NORMAL;
    fnt->outline_thickness = 0.0f;
    draw_string_internal(x, y, string, fnt, color, out_x, out_y);
}

void glez_circle(float x, float y, float radius, glez_rgba_t color,
                 float thickness, int steps)
{
    float px = 0;
    float py = 0;
    for (int i = 0; i < steps; i++)
    {
        float ang = 2 * M_PI * ((float) i / steps);
        if (!i)
            ang = 2 * M_PI * ((float) (steps - 1) / steps);
        if (i)
            glez_line(px, py, x - px + radius * cos(ang),
                      y - py + radius * sin(ang), color, thickness);
        px = x + radius * cos(ang);
        py = y + radius * sin(ang);
    }
}
