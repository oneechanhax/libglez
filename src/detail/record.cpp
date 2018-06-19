/*
  Created on 19.06.18.
*/

#include <cstddef>
#include <glez/detail/record.hpp>
#include <glez/record.hpp>

namespace glez::detail::record
{

void RecordedCommands::drawSegment(std::size_t index)
{
    glDrawElements(GL_TRIANGLES, segments[index].size, GL_UNSIGNED_INT, (void *)(segments[index].start * sizeof(glez::detail::render::vertex)));
}

void RecordedCommands::render()
{
    vertex_buffer_render_setup(vertex_buffer, GL_TRIANGLES);
    vertex_buffer_render_finish(vertex_buffer);
}

void RecordedCommands::bind(uint32_t texture)
{
    if (currentTexture != texture)
    {
        segments.push_back(segment{ nextStart, vertex_buffer->indices->size - nextStart, texture });
        currentTexture = texture;
    }
}

void
RecordedCommands::store(glez::detail::render::vertex *vertices, size_t vcount,
                        uint32_t *indices, size_t icount)
{
    vertex_buffer_push_back(vertex_buffer, vertices, vcount, indices, icount);
}

RecordedCommands::RecordedCommands()
{
    vertex_buffer = vertex_buffer_new("vertex:2f,tex_coord:2f,color:4f,drawmode:1i");
}

RecordedCommands::~RecordedCommands()
{
    vertex_buffer_delete(vertex_buffer);
}

void RecordedCommands::reset()
{
    vertex_buffer_clear(vertex_buffer);
    segments.clear();
}

RecordedCommands *currentRecord{ nullptr };

}

glez::record::Record::Record()
{
    commands = new glez::detail::record::RecordedCommands{};
}

glez::record::Record::~Record()
{
    delete commands;
}
