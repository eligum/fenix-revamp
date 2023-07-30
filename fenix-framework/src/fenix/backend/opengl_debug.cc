#include "fenix/backend/opengl_debug.hh"
#include <iostream>

namespace fenix::debug {

    void SetDebugOutputLogLevel(DebugLogLevel level)
    {
        GLenum severity;
        switch (static_cast<int>(level)) {
            case 1: severity = GL_DEBUG_SEVERITY_HIGH; break;
            case 2: severity = GL_DEBUG_SEVERITY_MEDIUM; break;
            case 3: severity = GL_DEBUG_SEVERITY_LOW; break;
            case 4: severity = GL_DEBUG_SEVERITY_NOTIFICATION; break;
            default: return;
        }
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, severity, 0, nullptr, GL_TRUE);
    }

    void DebugOutputCallBack(GLenum source,
                             GLenum type,
                             GLuint id,
                             GLenum severity,
                             GLsizei /* length */,
                             const GLchar* message,
                             const void* /* user_param */)
    // clang-format off
    {
        // Ignore non-significant error/warning codes.
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        std::cerr << "--------------------" << std::endl;
        std::cerr << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:             std::cerr << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Source: Other"; break;
        }
        std::cerr << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               std::cerr << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cerr << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cerr << "Type: Other"; break;
        }
        std::cerr << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Severity: notification"; break;
        }
        std::cerr << std::endl;
    }
    // clang-format on

    void EnableDebugOutput()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugOutputCallBack, nullptr);
    }

} // namespace fenix::debug
