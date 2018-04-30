/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/draw.hpp>
#include <glez/font.hpp>
#include <glez/detail/render.hpp>
#include <glez/detail/program.hpp>
#include <vertex-buffer.h>
#include <glez/detail/font.hpp>
#include <cstring>

namespace indices
{

static GLuint rectangle[6] = { 0, 1, 2, 2, 3, 0 };

}

void internal_draw_string(int x, int y, const std::string& string, texture_font_t *fnt, glez::rgba color, int *width, int *height)
{
    float pen_x  = x;
    float pen_y  = y + fnt->height / 1.5f;
    float size_y = 0;

    if (fnt->atlas->id == 0)
    {
        glGenTextures(1, &fnt->atlas->id);
    }

    glez::detail::render::bind(fnt->atlas->id);

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

    const char *sstring = string.c_str();

    for (size_t i = 0; i < string.size(); ++i)
    {
        texture_glyph_t *glyph = texture_font_find_glyph(fnt, &sstring[i]);
        if (glyph == NULL)
        {
            texture_font_load_glyph(fnt, &sstring[i]);
            continue;
        }
        glez::detail::render::vertex vertices[4];
        for (auto& vertex: vertices)
        {
            vertex.color = color;
            vertex.mode = static_cast<int>(glez::detail::program::mode::FREETYPE);
        }

        if (i > 0)
        {
            x += texture_glyph_get_kerning(glyph, &sstring[i - 1]);
        }

        float x0 = (int)(pen_x + glyph->offset_x);
        float y0 = (int)(pen_y - glyph->offset_y);
        float x1 = (int)(x0 + glyph->width);
        float y1 = (int)(y0 + glyph->height);
        float s0 = glyph->s0;
        float t0 = glyph->t0;
        float s1 = glyph->s1;
        float t1 = glyph->t1;

        vertices[0].position = {x0, y0};
        vertices[0].uv = {s0, t0};

        vertices[1].position = {x0, y1};
        vertices[1].uv = {s0, t1};

        vertices[2].position = {x1, y1};
        vertices[2].uv = {s1, t1};

        vertices[3].position = {x1, y0};
        vertices[3].uv = {s1, t0};

        pen_x += glyph->advance_x;
        //pen_x = (int) pen_x + 1;
        if (glyph->height > size_y)
            size_y = glyph->height;

        vertex_buffer_push_back(glez::detail::program::buffer, vertices, 4, indices::rectangle, 6);
    }

    if (width)
        *width = int(pen_x - x);
    if (height)
        *height = int(size_y);
}

namespace glez::draw
{


void line(int x, int y, int dx, int dy, rgba color, int thickness)
{

}

void rect(int x, int y, int w, int h, rgba color)
{
    detail::render::vertex vertices[4];

    for (auto &vertex : vertices)
    {
        vertex.mode = static_cast<int>(detail::program::mode::PLAIN);
        vertex.color = color;
    }

    vertices[0].position = { x, y };
    vertices[1].position = { x, y + h };
    vertices[2].position = { x + w, y + h };
    vertices[3].position = { x + w, y };

    ftgl::vertex_buffer_push_back(detail::program::buffer, vertices, 4, indices::rectangle, 6);
}

void rect_outline(int x, int y, int w, int h, rgba color, int thickness)
{

}

void circle(int x, int y, int radius, rgba color, int thickness,
                  int steps)
{

}

void string(int x, int y, const std::string &string, font& font,
             rgba color, int *width, int *height)
{
    if (!font.isLoaded())
        font.load();

    auto fnt = glez::detail::font::get(font.getHandle()).font;
    fnt->rendermode = RENDER_NORMAL;
    fnt->outline_thickness = 0.0f;
    internal_draw_string(x, y, string, fnt, color, width, height);

}

void outlined_string(int x, int y, const std::string &string,
                     font& font, rgba color,
                           rgba outline, int *width, int *height)
{
    if (!font.isLoaded())
        font.load();

    auto fnt = glez::detail::font::get(font.getHandle()).font;
    fnt->rendermode = RENDER_OUTLINE_POSITIVE;
    fnt->outline_thickness = 1.0f;
    internal_draw_string(x, y, string, fnt, outline, width, height);
    fnt->rendermode = RENDER_NORMAL;
    fnt->outline_thickness = 0.0f;
    internal_draw_string(x, y, string, fnt, color, width, height);
}

void
rect_textured(int x, int y, int w, int h, rgba color, texture &texture, int tx, int ty, int tw, int th)
{
    if (!texture.isLoaded())
        texture.load();

    detail::render::vertex vertices[4];

    for (auto &vertex : vertices)
    {
        vertex.mode = static_cast<int>(detail::program::mode::TEXTURED);
        vertex.color = color;
    }

    vertices[0].position = { x, y };
    vertices[1].position = { x, y + h };
    vertices[2].position = { x + w, y + h };
    vertices[3].position = { x + w, y };

    float s0 = tx / texture.getWidth();
    float s1 = (tx + tw) / texture.getWidth();
    float t0 = ty / texture.getHeight();
    float t1 = (ty + th) / texture.getHeight();

    vertices[0].uv = {s0, t1};
    vertices[1].uv = {s0, t0};
    vertices[2].uv = {s1, t0};
    vertices[3].uv = {s1, t1};

    ftgl::vertex_buffer_push_back(detail::program::buffer, vertices, 4, indices::rectangle, 6);
}


}