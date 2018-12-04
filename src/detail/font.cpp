/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/detail/font.hpp>
#include <vector>
#include <memory>
#include <cassert>

static std::unique_ptr<std::vector<glez::detail::font::font>> cache{ nullptr };

namespace glez::detail::font
{

void init()
{
    cache = std::make_unique<std::vector<font>>();
}

void shutdown()
{
    cache.reset(nullptr);
}

void font::load(const std::string &path, float size)
{
    assert(size > 0);

    atlas = texture_atlas_new(1024, 1024, 1);
    assert(atlas != nullptr);

    font = texture_font_new_from_file(atlas, size, path.c_str());
    assert(font != nullptr);

    init = true;
}

void font::unload()
{
    if (!init)
        return;
    texture_atlas_delete(atlas);
    if (font)
        texture_font_delete(font);
    init = false;
}

void font::stringSize(const std::string &string, float *width, float *height)
{
    float penX = 0;

    float size_x = 0;
    float size_y = 0;

    texture_font_load_glyphs(font, string.c_str());

    const char *sstring = string.c_str();

    for (size_t i = 0; i < string.size(); ++i)
    {
        // c_str guarantees a NULL terminator
        texture_glyph_t *glyph = texture_font_find_glyph(font, &sstring[i]);
        if (glyph == nullptr)
            continue;

        penX += texture_glyph_get_kerning(glyph, &sstring[i]);
        penX += glyph->advance_x;

        if (penX > size_x)
            size_x = penX;

        if (glyph->height > size_y)
            size_y = glyph->height;
    }
    if (width)
        *width = size_x;
    if (height)
        *height = size_y;
}

unsigned create()
{
    for (size_t i = 0; i < cache->size(); ++i)
        if (not(*cache)[i].init)
            return i;
    auto result = cache->size();
    cache->push_back(font{});
    return result;
}

font &get(unsigned handle)
{
    return (*cache)[handle];
}
} // namespace glez::detail::font
