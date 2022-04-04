/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <cassert>
#include <cstdio>
#include <cstring>
#include <freetype-gl.h>
#include <glez/detail/record.hpp>
#include <glez/font.hpp>
#include <glez/glez.hpp>
#include <mat4.h>
#include <stdexcept>
#include <vertex-buffer.h>

static const char* shader_vertex = R"END(
#version 130
uniform mat4 projection;
in vec2 vertex;
in vec2 tex_coord;
in vec4 color;
in int drawmode;
flat out int frag_DrawMode;
out vec4 frag_Color;
out vec2 frag_TexCoord;
void main()
{
    gl_Position   = projection*vec4(vertex + vec2(0.0, -0.05),0.0,1.0);
    frag_TexCoord = tex_coord;
    frag_Color = color;
    frag_DrawMode = drawmode;
}
)END";

static const char* shader_fragment = R"END(
#version 130
uniform sampler2D texture;
in vec4 frag_Color;
in vec2 frag_TexCoord;
flat in int frag_DrawMode;
void main()
{
   if (frag_DrawMode == 1)
       gl_FragColor = frag_Color;
   else
   {
       vec4 tex = texture2D(texture, frag_TexCoord);
       if (frag_DrawMode == 2)
           gl_FragColor = frag_Color * tex;
       else if (frag_DrawMode == 3)
       {
           gl_FragColor = vec4(frag_Color.rgb, frag_Color.a * tex.r);
       }
       else
           gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
)END";

static GLuint shader { 0 };

static GLuint compile(const char* source, GLenum type) {
    GLint status;
    GLuint result = glCreateShader(type);

    glShaderSource(result, 1, &source, 0);
    glCompileShader(result);
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char error[512];
        GLsizei length;
        glGetShaderInfoLog(result, 512, &length, error);
        fprintf(stderr, "GLEZ: Shader compile error: %s, %s\n", error, source);
        throw std::runtime_error("Shader compile error: " + std::string(error));
    }

    return result;
}

static GLuint link(GLuint vertex, GLuint fragment) {
    GLuint result = glCreateProgram();
    GLint status;

    glAttachShader(result, vertex);
    glAttachShader(result, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    glLinkProgram(result);
    glGetProgramiv(result, GL_LINK_STATUS, &status);

    assert(status == GL_TRUE);

    return result;
}

namespace glez {

vertex_buffer_t* buffer { nullptr };

void resize(int width, int height) {
    glUseProgram(shader);
    mat4 projection;
    mat4_set_identity(&projection);
    mat4_set_orthographic(&projection, 0, width, height, 0, -1, 1);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, 0, projection.data);
    glUseProgram(0);
}

void init(int width, int height) {
    buffer = ftgl::vertex_buffer_new("vertex:2f,tex_coord:2f,color:4f,drawmode:1i");
    shader = link(compile(shader_vertex, GL_VERTEX_SHADER), compile(shader_fragment, GL_FRAGMENT_SHADER));

    resize(width, height);
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "texture"), 0);
    glUseProgram(0);
}

void shutdown() {
    ftgl::vertex_buffer_delete(buffer);
    glDeleteProgram(shader);
}

static GLuint current_texture { 0 };

void begin() {
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);
    glDisableClientState(GL_FOG_COORD_ARRAY);
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    current_texture = 0;

    glUseProgram(shader);
}

void end() {
    vertex_buffer_render(buffer, GL_TRIANGLES);
    ftgl::vertex_buffer_clear(buffer);
    glUseProgram(0);
    glPopClientAttrib();
    glPopAttrib();
}

void bind(GLuint texture) {
    if (current_texture != texture) {
        if (!detail::record::isReplaying) {
            vertex_buffer_render(buffer, GL_TRIANGLES);
            ftgl::vertex_buffer_clear(buffer);
        }
        current_texture = texture;
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

} // namespace glez
