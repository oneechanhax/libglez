/*
  Created on 19.06.18.
*/

#pragma once

#include <vector>
#include <cstdint>
#include "render.hpp"
#include "../../../ftgl/vertex-buffer.h"

namespace glez::detail::record
{

class RecordedCommands
{
public:
    struct segment
    {
        std::size_t start{};
        std::size_t size{};
        uint32_t texture{};
    };

    RecordedCommands();
    ~RecordedCommands();

    void reset();
    void store(glez::detail::render::vertex *vertices, size_t vcount, uint32_t *indices, size_t icount);
    void bind(uint32_t texture);
    void render();
protected:
    void drawSegment(std::size_t index);

    uint32_t currentTexture{ 0 };
    size_t nextStart{ 0 };
    ftgl::vertex_buffer_t *vertex_buffer{};
    std::vector<segment> segments{};
};

extern RecordedCommands *currentRecord;

}