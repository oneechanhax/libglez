/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <cassert>
#include <cstring>
#include <fstream>
#include <memory>
#include <vector>

#include "picopng/picopng.hpp"

#include <glez/glez.hpp>
#include <glez/texture.hpp>

namespace glez {

void texture::bind() {
    if (!bound) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        bound = true;
    }
    glez::bind(id);
}

texture texture::loadFromFile(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::streamsize size = 0;
    if (file.seekg(0, std::ios::end).good())
        size = file.tellg();
    if (file.seekg(0, std::ios::beg).good())
        size -= file.tellg();

    if (size < 1)
        throw std::runtime_error("Unable to load texture from file!");

    unsigned char* buffer = new unsigned char[(size_t)size + 1];
    file.read((char*)buffer, size);
    file.close();
    texture ret;
    int error = decodePNG(ret.data, ret.width, ret.height, buffer, size);

    // if there's an error, display it and return false to indicate failure
    if (error != 0) {
        printf("Error loading texture, error code %i\n", error);
        throw std::runtime_error("See above!");
    }
    ret.init = true;
    ret.bound = false;
    ret.id = 0;
    return ret;
}

texture texture::loadFromMemory(const std::byte* mem, std::size_t size, unsigned w, unsigned h) {
    if (size < 1)
        throw std::runtime_error("Unable to load texture from memory!");

    unsigned char* buffer = new unsigned char[size + 1];
    std::memcpy(buffer, mem, size);

    texture ret;
    ret.width = w;
    ret.height = h;
    ret.data = buffer;
    ret.init = true;
    ret.bound = false;
    ret.id = 0;
    return ret;
}

texture::~texture() {
    if (bound)
        glDeleteTextures(1, &id);
    if (init)
        delete[] data;
    init = false;
}

texture& texture::operator=(texture&& var) {
    this->init = var.init;
    this->bound = var.bound;
    this->width = var.width;
    this->height = var.height;
    this->id = var.id;
    this->data = var.data;

    var.init = false;
    var.bound = false;
    var.width = 0;
    var.height = 0;
    var.id = 0;
    var.data = nullptr;
    return *this;
}

} // namespace glez
