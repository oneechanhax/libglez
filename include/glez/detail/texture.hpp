/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <GL/gl.h>
#include <string>
#include <limits>
#include <glez/types.hpp>

namespace glez::detail::texture
{

struct texture
{
    void bind();
    void load(const std::string& path);
    void unload();

    bool init{ false };
    bool bound{ false };

    int width{ 0 };
    int height{ 0 };

    GLuint id;
    GLubyte *data;
};

void init();
void shutdown();

glez::types::handle_type create();
texture& get(glez::types::handle_type handle);

}