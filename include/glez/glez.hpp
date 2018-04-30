/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>

namespace glez
{

constexpr unsigned max_textures = 32;
constexpr unsigned max_fonts = 32;

void init(int width, int height);
void shutdown();

void resize(int width, int height);

void begin();
void end();

namespace draw
{

void line(int x, int y, int dx, int dy, int color, int thickness);
void rect(int x, int y, int w, int h, int color);
void rect_outline(int x, int y, int w, int h, int color, int thickness);
void circle(int x, int y, int radius, int color, int thickness, int steps);

void string(int x, int y, const std::string& string, int color, int *width, int *height);
void outlined_string(int x, int y, const std::string& string, int color, int *width, int *height);

}
};