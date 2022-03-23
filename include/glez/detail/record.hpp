/*
  Created on 19.06.18.
*/

#pragma once

#include <vector>
#include <cstdint>
#include <glez/texture.hpp>
#include <glez/font.hpp>
#include <glez/glez.hpp>
#include <vertex-buffer.h>
#include <glez/font.hpp>
#include <freetype-gl.h>

namespace glez::detail::record {

class RecordedCommands {
public:
    struct segment
    {
        std::size_t start{ 0 };
        std::size_t size{ 0 };
        glez::texture *texture{ nullptr };
        glez::font *font{ nullptr };
    };

    RecordedCommands();
    ~RecordedCommands();

    void reset();
    void store(glez::vertex *vertices, size_t vcount,
               uint32_t *indices, size_t icount);
    void bindTexture(glez::texture *tx);
    void bindFont(glez::font *font);
    void render();
    void end();

protected:
    void cutSegment();

    ftgl::vertex_buffer_t *vertex_buffer{};
    std::vector<segment> segments{};
    segment current{};
};

extern RecordedCommands *currentRecord;
extern bool isReplaying;

} // namespace glez::detail::record
