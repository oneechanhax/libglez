/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>
#include "types.hpp"

namespace glez::draw
{

void line(int x, int y, int dx, int dy, types::rgba color, int thickness);
void rect(int x, int y, int w, int h, types::rgba color);
void rect_outline(int x, int y, int w, int h, types::rgba color, int thickness);
void circle(int x, int y, int radius, types::rgba color, int thickness, int steps);

void string(int x, int y, const std::string& string, types::handle_type font, types::rgba color, int *width, int *height);
void outlined_string(int x, int y, const std::string& string, types::handle_type font, types::rgba color, types::rgba outline, int *width, int *height);

}