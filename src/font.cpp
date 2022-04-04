/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <cassert>
#include <glez/font.hpp>
#include <memory>
#include <vector>

namespace glez {

font font::loadFromFile(const std::string& path, float size) {
    assert(size > 0);

    font ret;
    ret.atlas = texture_atlas_new(1024, 1024, 1);
    assert(ret.atlas != nullptr);

    ret.m_font = texture_font_new_from_file(ret.atlas, size, path.c_str());
    assert(ret.m_font != nullptr);
    return ret;
}

font::~font() {
    if (atlas)
        texture_atlas_delete(atlas);
    if (m_font)
        texture_font_delete(m_font);
}
glez::font& font::operator=(glez::font&& var) {
    this->m_font = var.m_font;
    this->atlas = var.atlas;
    var.m_font = nullptr;
    var.atlas = nullptr;
    return *this;
}
void font::stringSize(const std::string& string, float* width, float* height) {
    assert(this->isLoaded());
    float penX = 0;

    float size_x = 0;
    float size_y = 0;

    texture_font_load_glyphs(m_font, string.c_str());

    const char* sstring = string.c_str();

    for (size_t i = 0; i < string.size(); ++i) {
        // c_str guarantees a NULL terminator
        texture_glyph_t* glyph = texture_font_find_glyph(m_font, &sstring[i]);
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

} // namespace glez
