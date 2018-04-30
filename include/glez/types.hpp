/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <limits>

namespace glez::types
{

struct rgba
{
    union
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        float data[4];
    };
};

struct cached_shape
{
private:
    cached_shape();
public:
    ~cached_shape();

    const unsigned texture;
    const void *const data;
};

using handle_type = unsigned;
constexpr handle_type undefined = std::numeric_limits<handle_type>::max();

}