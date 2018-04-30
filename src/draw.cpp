/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/draw.hpp>
#include <glez/detail/render.hpp>
#include <glez/detail/program.hpp>
#include <vertex-buffer.h>

namespace indices
{

static GLuint rectangle[6] = { 0, 1, 2, 2, 3, 0 };

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

void string(int x, int y, const std::string &string, const font& font,
             rgba color, int *width, int *height)
{

}

void outlined_string(int x, int y, const std::string &string,
                     const font& font, rgba color,
                           rgba outline, int *width, int *height)
{

}


}