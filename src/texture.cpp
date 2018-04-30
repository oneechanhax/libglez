/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/detail/texture.hpp>
#include <glez/detail/render.hpp>
#include <cassert>
#include <glez/glez.hpp>

static glez::detail::texture::texture cache[glez::max_textures] {};

namespace glez::detail::texture
{

void init()
{
}

void shutdown()
{
}

void texture::bind()
{
    if (not bound)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, &id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    render::bind(id);
}

bool texture::load(const std::string &type)
{

}

texture& get(glez::types::handle_type handle)
{
    assert(handle < glez::max_textures);
    assert(cache[handle].init);

    return cache[handle];
}

}