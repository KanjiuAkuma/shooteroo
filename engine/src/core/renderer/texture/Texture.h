#pragma once

/**
  * Created by Joscha Vack on 11/22/2020.
  *
  **/

#include <string>
#include <glm/glm.hpp>

class Texture {
public:
    explicit Texture(const std::string& filePath);
    ~Texture();


    void bind(unsigned int slot);
    void unbind();

private:
    unsigned int renderId = 0;
};
