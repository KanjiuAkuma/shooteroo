/**
  * Created by Joscha Vack on 11/22/2020.
  *
  **/

#include "Texture.h"
#include <Macros.h>
#include <stb_image.h>

Texture::Texture(const std::string& filePath) {
    // load image to buffer
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* buffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);

    // create texture
    GL_CALL(glGenTextures(1, &renderId));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, renderId));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer))
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0))

    // free image buffer
    if (buffer) stbi_image_free(buffer);
    LOG_DEBUG("Created texture {} from file {}", renderId, filePath);
}

Texture::~Texture() {
    ASSERT(renderId != 0, "Attempting to delete texture without render id")
    unsigned int id = renderId;
    GL_CALL(glDeleteTextures(1, &renderId));

    LOG_DEBUG("Deleted texture {}", id);
}

void Texture::bind(unsigned int slot) {
    ASSERT(renderId != 0, "Attempting to bind texture without render id")
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot))
    GL_CALL(glBindTexture(GL_TEXTURE_2D, renderId))
}

void Texture::unbind() {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0))
}
