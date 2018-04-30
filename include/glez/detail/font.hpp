/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <freetype-gl.h>
#include <limits>
#include <glez/types.hpp>

namespace glez::detail::font
{

struct font
{
    bool init{ false };

    texture_font_t *font{ nullptr };
    texture_atlas_t *atlas{ nullptr };
};

void init();
void shutdown();

font *get(glez::types::handle_type handle);

}