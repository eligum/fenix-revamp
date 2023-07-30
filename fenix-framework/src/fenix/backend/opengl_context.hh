#pragma once

// Placeholder struct (forward declaration) to avoid including glfw here
struct GLFWwindow;

namespace fenix {

    /// Wrapper for requesting and OpenGL context for an existing GLFW window.
    class OpenGLContext
    {
    public:
        OpenGLContext(GLFWwindow* window_handle);
        ~OpenGLContext();

        void Init();
    private:
        GLFWwindow* m_WindowHandle;
    };

} // namespace fenix
