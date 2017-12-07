#pragma once

#include <vertex-buffer.h>

enum
{
    DRAW_MODE_PLAIN = 1,
    DRAW_MODE_TEXTURED,
    DRAW_MODE_FREETYPE
};

struct program_t
{
    char shader_active;
    unsigned shader;
    vertex_buffer_t *buffer;
};

struct program_t program;

void shader_screen_size(int width, int height);

void program_init(int width, int height);

void program_draw();

void program_reset();

unsigned program_next_index();
