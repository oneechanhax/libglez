/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <vector>
#include "render.hpp"

namespace glez::detail::builder
{

class Builder
{
public:
    Builder(int mode, GLuint texture);

    Builder& setColor(types::rgba color);

    Builder& push(float x, float y, float u, float v);
    Builder& push(float x, float y);
protected:
    types::rgba color;

    GLuint texture{ 0 };
    int mode{ 0 };
    std::vector<render::vertex> vertices{};
};

}