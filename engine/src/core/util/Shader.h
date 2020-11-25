#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <glad/glad.h>
#include <string>

namespace Engine { namespace Util {

    class Shader {
    public:
        ~Shader();

        GLuint getRendererID() { return rendererID; }

        static Shader* fromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    private:
        Shader() = default;

        void loadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint compileShader(GLenum type, const std::string& source);
    private:
        GLuint rendererID;
    };

}}