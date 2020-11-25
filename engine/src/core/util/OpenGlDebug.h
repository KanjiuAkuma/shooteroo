#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <glad/glad.h>

namespace Engine { namespace Util {

    enum class DebugLogLevel {
        None = 0, HighAssert = 1, High = 2, Medium = 3, Low = 4, Notification = 5
    };

        void enableGLDebugging();
        void setGLDebugLogLevel(DebugLogLevel level);
        void openGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

}}
