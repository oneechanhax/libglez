/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/detail/texture.hpp>
#include <glez/detail/render.hpp>
#include <cassert>
#include <vector>
#include <png.hpp>
#include <memory>

static std::unique_ptr<std::vector<glez::detail::texture::texture>> cache{
    nullptr
};

namespace glez::detail::texture
{

void init()
{
    cache = std::make_unique<std::vector<texture>>();
}

void shutdown()
{
    cache.reset(nullptr);
}

void texture::bind()
{
    if (not bound)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        bound = true;
    }

    render::bind(id);
}

void texture::load(const std::string &path)
{
    try
    {
        png::image<png::rgba_pixel> image(path);
        width      = image.get_width();
        height     = image.get_height();
        init       = true;
        bound      = false;
        id         = 0;
        auto bytes = image.get_width() * image.get_height() * 4;
        data       = new GLubyte[bytes];

        for (int i = 0; i < image.get_height(); ++i)
        {
            memcpy(data + image.get_width() * 4 * i,
                   image.get_pixbuf().get_row(i).data(), image.get_width() * 4);
        }
    }
    catch (...)
    {
      
    }
}

void texture::unload()
{
    if (bound)
        glDeleteTextures(1, &id);
    if (init)
        delete[] data;
    init = false;
}

texture &get(unsigned handle)
{
    return (*cache)[handle];
}

unsigned create()
{
    for (auto i = 0u; i < cache->size(); ++i)
        if (not(*cache)[i].init)
            return i;
    auto result = cache->size();
    cache->push_back(texture{});
    return result;
}
}
