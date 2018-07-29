/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/font.hpp>
#include <glez/detail/font.hpp>

namespace glez
{

font::~font()
{
    if (loaded)
        unload();
}

void font::load()
{
    handle     = detail::font::create();
    auto &font = detail::font::get(handle);
    font.load(path, size);
    loaded = true;
}

void font::unload()
{
    auto &font = detail::font::get(handle);
    font.unload();
}

void font::stringSize(const std::string &string, float *width, float *height)
{
    if (!isLoaded())
        load();
    auto &font = detail::font::get(handle);
    font.stringSize(string, width, height);
}
}