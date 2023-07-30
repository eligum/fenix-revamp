#pragma once

#include <glad/glad.h>
#include "fenix/core/log.hh"

namespace fenix::debug {

    enum class DebugLogLevel
    {
        None         = 0,
        High         = 1,
        Medium       = 2,
        Low          = 3,
        Notification = 4
    };

    void EnableDebugOutput();
    void SetDebugOutputLogLevel(DebugLogLevel level);
    void DebugOutputCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

} // namespace fenix::debug
