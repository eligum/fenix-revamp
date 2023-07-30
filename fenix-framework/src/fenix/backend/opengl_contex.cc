#include "fenix/backend/opengl_context.hh"
#include "fenix/backend/opengl_debug.hh"
#include "fenix/core/base.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace fenix {

    OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
        : m_WindowHandle(window_handle)
    {
        FENIX_ASSERT(window_handle, "Window handle is null!");
    }

    OpenGLContext::~OpenGLContext()
    {
        glfwTerminate();
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        [[maybe_unused]] i32 status =
            gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        FENIX_ASSERT(status > 0, "Failed to initialize GLAD!");

        CORE_LOG_INFO("------------");
        CORE_LOG_INFO("Vendor:   {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        CORE_LOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        CORE_LOG_INFO("Version:  {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        CORE_LOG_INFO("------------");

        FENIX_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "FENIX requires at least OpenGL version 4.5");

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            CORE_LOG_WARN("Using an OpenGL debug context, GL operations will be significantly slower.");
            CORE_LOG_WARN("If this is not intentional you should request a normal context.");
            debug::EnableDebugOutput();
            debug::SetDebugOutputLogLevel(debug::DebugLogLevel::Notification);
        }
    }

} // namespace fenix
