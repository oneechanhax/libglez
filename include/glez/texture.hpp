/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <cstddef>
#include <limits>
#include <string>

#include <freetype-gl.h>

namespace glez {

class texture {
public:
    texture() { }
    static texture loadFromFile(const std::string& path);
    static texture loadFromMemory(const std::byte* mem, std::size_t size, unsigned w, unsigned h);
    texture(texture&&);
    ~texture();

    texture& operator=(texture&&);

    void bind();

    inline bool isLoaded() { return this->init; }

public:
    bool init = false;
    bool bound = false;
    int width = 0;
    int height = 0;

    GLuint id;
    GLubyte* data;
};

} // namespace glez
