/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <limits>
#include <string>

#include <freetype-gl.h>

namespace glez {

class font {
public:
    font() { }
    static font loadFromFile(const std::string& path, float size);
    font(font&&);
    ~font();

    glez::font& operator=(glez::font&&);

    // void stringSize(std::string_view string, float* width, float* height);
    void stringSize(const std::string& string, float* width, float* height) const;
    inline bool isLoaded() const { return this->m_font != nullptr && this->atlas != nullptr; };

public:
    texture_font_t* m_font = nullptr;
    texture_atlas_t* atlas = nullptr;
};

} // namespace glez
