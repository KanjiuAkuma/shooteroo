/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "OpenGlDebug.h"

#include "Macros.h"
#include "core/logger/Logger.h"

namespace Engine { namespace Util {

    static DebugLogLevel debugLogLevel = DebugLogLevel::HighAssert;

    void setDebugLogLevel(DebugLogLevel level) {
        debugLogLevel = level;
    }

    void GLAPIENTRY openGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                if ((int) debugLogLevel > 0) {
                    LOG_ERROR("[OpenGL Debug HIGH] {}", message);
                    if (debugLogLevel == DebugLogLevel::HighAssert) CORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
                }
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                if ((int) debugLogLevel > 2)
                    LOG_WARN("[OpenGL Debug MEDIUM] {}", message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                if ((int) debugLogLevel > 3)
                    LOG_INFO("[OpenGL Debug LOW] {}", message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                if ((int) debugLogLevel > 4)
                    LOG_TRACE("[OpenGL Debug NOTIFICATION] {}", message);
                break;
        }
    }

    void enableGLDebugging() {
        glDebugMessageCallback(openGLLogMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

}}