/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "Shader.h"

#include <core/logger/Logger.h>

#include <fstream>

namespace Engine { namespace Util {

    static std::string readFileAsString(const std::string& filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LOG_ERROR("Could not open file '{}'", filepath);
        }

        return result;
    }

    Shader::~Shader() {
        glDeleteProgram(rendererID);
    }

    GLuint Shader::compileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            LOG_ERROR("{0}", infoLog.data());
        }

        return shader;
    }

    Shader* Shader::fromGLSLTextFiles(const std::string & vertexShaderPath, const std::string & fragmentShaderPath) {
        Shader* shader = new Shader();
        shader->loadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
        return shader;
    }

    void Shader::loadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
        std::string vertexSource = readFileAsString(vertexShaderPath);
        std::string fragmentSource = readFileAsString(fragmentShaderPath);

        GLuint program = glCreateProgram();
        int glShaderIDIndex = 0;

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            LOG_ERROR("{0}", infoLog.data());
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        rendererID = program;
    }

}}