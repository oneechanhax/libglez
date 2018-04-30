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
    bool load(const std::string& type);

    bool init{ false };
    bool bound{ false };

    GLuint id;
    GLubyte *data;

    int width{ 0 };
    int height{ 0 };

    char filename[256];
};

void init();
void shutdown();

texture& get(glez::types::handle_type handle);

}