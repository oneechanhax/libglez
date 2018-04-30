/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>
#include "types.hpp"

namespace glez
{

class font
{
public:
    inline font(std::string path, float size)
            : path(std::move(path)), size(size)
    {
    }
    ~font();

    inline float getSize() const
    {
        return size;
    }

    inline const std::string& getPath() const
    {
        return path;
    }
protected:
    void load();
    void unload();

    const std::string path;
    const float size;

    bool loaded{ false };

    glez::types::handle_type handle{ glez::types::undefined };
};

}