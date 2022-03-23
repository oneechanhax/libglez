/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <vertex-buffer.h>
#include <freetype-gl.h>
#include <glez/color.hpp>

namespace glez {

void init(int width, int height);
void shutdown();
void resize(int width, int height);
void begin();
void end();

enum class mode {
    PLAIN = 1,
    TEXTURED,
    FREETYPE
};

extern ftgl::vertex_buffer_t *buffer;
struct vertex {
    ftgl::vec2 position;
    ftgl::vec2 uv;
    rgba color;
    int mode;
};

void bind(GLuint texture);

}; // namespace glez::detail::program
