/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>
#include "types.hpp"

namespace glez
{

class texture
{
public:
    inline explicit texture(std::string path)
            : path(std::move(path))
    {
    }
    ~texture();

    inline int getWidth() const
    {
        return width;
    }

    inline int getHeight() const
    {
        return height;
    }

    inline const std::string& getPath() const
    {
        return path;
    }
protected:
    void load();
    void unload();

    const std::string path;

    int width{ 0 };
    int height{ 0 };
    bool loaded{ false };

    glez::types::handle_type handle{ glez::types::undefined };
};

}