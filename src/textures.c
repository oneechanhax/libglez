/*
 * textures.c
 *
 *  Created on: Dec 7, 2017
 *      Author: nullifiedcat
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include "glez.h"
#include "internal/draw.h"
#include "internal/textures.h"

#include <assert.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <libpng/png.h>

static internal_texture_t loaded_textures[64];

void internal_textures_init()
{
    memset(loaded_textures, 0, sizeof(loaded_textures));
}

void internal_textures_destroy()
{
}

int internal_texture_load_png_rgba(const char *name, internal_texture_t *out)
{
    memset(out, 0, sizeof(internal_texture_t));

    FILE *file;
    file = fopen(name, "r");
    if (file == NULL)
    {
        perror("textureapi: could not open file: ");
        return -1;
    }
    png_byte header[8];
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8))
    {
        fclose(file);
        return -1;
    }
    png_structp pngstr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (pngstr == NULL)
    {
        fclose(file);
        return -1;
    }
    png_infop pnginfo = png_create_info_struct(pngstr);
    png_infop pngend  = png_create_info_struct(pngstr);
    if (setjmp(png_jmpbuf(pngstr)))
    {
        png_destroy_read_struct(&pngstr, &pnginfo, &pngend);
        fclose(file);
        return -1;
    }
    png_init_io(pngstr, file);
    png_set_sig_bytes(pngstr, 8);
    png_read_info(pngstr, pnginfo);
    int width, height, depth, colortype;
    png_get_IHDR(pngstr, pnginfo, &width, &height, &depth, &colortype, NULL,
                 NULL, NULL);
    png_read_update_info(pngstr, pnginfo);
    int row_bytes;
    if (PNG_COLOR_TYPE_RGBA != png_get_color_type(pngstr, pnginfo))
    {
        png_destroy_read_struct(&pngstr, &pnginfo, &pngend);
        fclose(file);
        return -1;
    }

    png_bytepp row_pointers;
    row_bytes = png_get_rowbytes(pngstr, pnginfo);
    out->data = malloc(row_bytes * height * sizeof(png_byte));
    if (out->data == NULL)
    {
        png_destroy_read_struct(&pngstr, &pnginfo, &pngend);
        fclose(file);
        return -1;
    }
    row_pointers = malloc(height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        png_destroy_read_struct(&pngstr, &pnginfo, &pngend);
        free(out->data);
        fclose(file);
        return -1;
    }
    for (int i = 0; i < height; ++i)
    {
        row_pointers[height - 1 - i] = out->data + i * row_bytes;
    }
    png_read_image(pngstr, row_pointers);

    free(row_pointers);
    fclose(file);

    out->width  = width;
    out->height = height;
    out->init   = 1;

    return 0;
}

internal_texture_t *internal_texture_get(glez_texture_t handle)
{
    assert(handle < 64);
    assert(loaded_textures[handle].init);

    return &loaded_textures[handle];
}

void internal_texture_bind(glez_texture_t handle)
{
    internal_texture_t *texture = internal_texture_get(handle);

    if (!texture->bound)
    {
        glGenTextures(1, &texture->texture_id);
        glBindTexture(GL_TEXTURE_2D, texture->texture_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture->bound = 1;
    }

    ds_bind_texture(texture->texture_id);
}

glez_texture_t glez_texture_load_png_rgba(const char *path)
{
    internal_texture_t result;

    memset(&result, 0, sizeof(result));
    strncpy(result.filename, path, 255);

    if (internal_texture_load_png_rgba(path, &result) != 0)
    {
        return GLEZ_TEXTURE_INVALID;
    }

    for (glez_texture_t i = 0; i < 64; ++i)
    {
        if (loaded_textures[i].init == 0)
        {
            memcpy(&loaded_textures[i], &result, sizeof(result));
            loaded_textures[i].init = 1;
            return i;
        }
    }

    return GLEZ_TEXTURE_INVALID;
}

void glez_texture_unload(glez_texture_t handle)
{
    internal_texture_t *tx = internal_texture_get(handle);

    glDeleteTextures(1, &tx->texture_id);
    free(tx->data);

    tx->init = 0;
}

void glez_texture_size(glez_texture_t handle, int *width, int *height)
{
    internal_texture_t *tx = internal_texture_get(handle);

    if (width)
        *width = tx->width;
    if (height)
        *height = tx->height;
}
