#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <glad/glad.h>

#include <core/logger/Logger.h>

#ifdef NDEBUG
// core macros
    #define GL_CALL(x) x;
    #define CORE_ASSERT(x, ...)
// client macros
    #define DEBUG(x)
    #define ASSERT(x, ...)
#else
// core macros
    #define GL_CALL(x) { glClearErrors(); x; glLogCall(#x, __FILE__, __LINE__); }
    #define CORE_ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Core assertion failed: {}", __VA_ARGS__, __FILE__, __LINE__); }}
// client macros
    #define DEBUG(x) x
    #define ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion failed: {} {}:{}", __VA_ARGS__, __FILE__, __LINE__); }}
#endif

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

inline void glClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

inline bool glLogCall(const char* function, const char* file, long line) {
    bool noErr = true;
    while (GLenum error = glGetError()) {
        LOG_ERROR("[OpenGL Error] {}: in {}.{}:{}\n", error, function, file, line);
        noErr = false;
    }
    return noErr;
}