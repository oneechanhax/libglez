#include <GL/glew.h>
#include <GL/gl.h>

#include <assert.h>
#include <stdio.h>

#include <mat4.h>

#include "internal/program.h"

GLuint compile_shader(const char *source, GLenum type)
{
    GLint status;
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
	{
    	char infolog[512];
    	GLsizei length;
    	glGetShaderInfoLog(shader, 512, &length, infolog);
    	fprintf(stderr, "GLEZ: Shader compile error: %s\n", infolog);
    	assert(status == GL_TRUE);
	}

    return shader;
}

const char *shader_ultimate_vert =
    "#version 130\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "in vec2 vertex;\n"
    "in vec2 tex_coord;\n"
    "in vec4 color;\n"
    "in int drawmode;\n"
    "flat out int frag_DrawMode;\n"
    "out vec4 frag_Color;\n"
    "out vec2 frag_TexCoord;\n"
    "void main()\n"
    "{\n"
    "    frag_TexCoord = tex_coord;\n"
    "    frag_Color    = color;\n"
    "    gl_Position   = projection*(view*(model*vec4(vertex,0.0,1.0)));\n"
    "    frag_DrawMode = drawmode;\n"
    "}";
const char *shader_ultimate_frag =
    "#version 130\n"
    "\n"
    "uniform sampler2D texture;\n"
    "in vec4 frag_Color;\n"
    "in vec2 frag_TexCoord;\n"
    "flat in int frag_DrawMode;\n"
    "void main()\n"
    "{\n"
    "   if (frag_DrawMode == 1)\n"
    "       gl_FragColor = frag_Color;\n"
    "   else\n"
    "   {\n"
    "       vec4 tex = texture2D(texture, frag_TexCoord);\n"
    "       if (frag_DrawMode == 2)\n"
    "           gl_FragColor = frag_Color * tex;\n"
    "       else if (frag_DrawMode == 3)\n"
    "       {\n"
    "           gl_FragColor = vec4(frag_Color.rgb, frag_Color.a * tex.r);\n"
    "       }\n"
    "       else\n"
    "           gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "    }\n"
    "}";

void shader_screen_size(int width, int height)
{
    glUseProgram(program.shader);
    mat4 projection;
    mat4_set_identity(&projection);
    mat4_set_orthographic(&projection, 0, width, height, 0, -1, 1);
    glUniformMatrix4fv(glGetUniformLocation(program.shader, "projection"), 1, 0,
                       projection.data);
    glUseProgram(0);
}

void program_init(int width, int height)
{
    program.buffer =
        vertex_buffer_new("vertex:2f,tex_coord:2f,color:4f,drawmode:1i");
    program.shader = glCreateProgram();
    GLint status;
    GLuint sh_frag = compile_shader(shader_ultimate_frag, GL_FRAGMENT_SHADER);
    glAttachShader(program.shader, sh_frag);
    glDeleteShader(sh_frag);

    GLuint sh_vert = compile_shader(shader_ultimate_vert, GL_VERTEX_SHADER);
    glAttachShader(program.shader, sh_vert);
    glDeleteShader(sh_vert);

    glLinkProgram(program.shader);
    glGetProgramiv(program.shader, GL_LINK_STATUS, &status);

    assert(status == GL_TRUE);

    glUseProgram(program.shader);

    mat4 model, view, projection;

    mat4_set_identity(&model);
    mat4_set_identity(&view);
    mat4_set_identity(&projection);
    mat4_set_orthographic(&projection, 0, width, height, 0, -1, 1);

    glUniformMatrix4fv(glGetUniformLocation(program.shader, "model"), 1, 0,
                       model.data);
    glUniformMatrix4fv(glGetUniformLocation(program.shader, "view"), 1, 0,
                       view.data);
    glUniformMatrix4fv(glGetUniformLocation(program.shader, "projection"), 1, 0,
                       projection.data);
    glUniform1i(glGetUniformLocation(program.shader, "texture"), 0);

    glUseProgram(0);
}

void program_draw()
{
    glUseProgram(program.shader);
    vertex_buffer_render(program.buffer, GL_TRIANGLES);
    glUseProgram(0);
}

void program_reset()
{
    vertex_buffer_clear(program.buffer);
}

unsigned program_next_index()
{
    return program.buffer->vertices->size;
}
