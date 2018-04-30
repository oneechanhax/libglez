/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>
#include "color.hpp"
#include "font.hpp"
#include "texture.hpp"

namespace glez::draw
{

void line(int x, int y, int dx, int dy, rgba color, int thickness);
void rect(int x, int y, int w, int h, rgba color);
void rect_outline(int x, int y, int w, int h, rgba color, int thickness);
void rect_textured(int x, int y, int w, int h, rgba color, texture& texture, int tx, int ty, int tw, int th, float angle);
void circle(int x, int y, int radius, rgba color, int thickness, int steps);

void string(int x, int y, const std::string& string, font& font, rgba color, int *width, int *height);
void outlined_string(int x, int y, const std::string& string, font& font, rgba color, rgba outline, int *width, int *height);

}