#include <glad/glad.h>

#include "fenix/backend/linux_window.hh"
#include "fenix/backend/opengl_context.hh"

#include "fenix/core/base.hh"
#include "fenix/events/application_event.hh"
#include "fenix/events/key_event.hh"
#include "fenix/events/mouse_event.hh"
#include "fenix/input/input.hh"

static void GLFWErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW error ({}): {}", error, description);
}

namespace fenix {

    static u8 s_GLFWWindowCount = 0;

    Window* Window::Create(const WindowProps& props)
    {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props)
    {
        Init(props);
    }

    LinuxWindow::~LinuxWindow()
    {
        Shutdown();
    }

    void LinuxWindow::Init(const WindowProps& props)
    {
        m_Data.Title  = props.title;
        m_Data.Width  = props.width;
        m_Data.Height = props.height;

        if (s_GLFWWindowCount == 0)
        {
            [[maybe_unused]] i32 success = glfwInit();
            FENIX_ASSERT(success, "Failed to initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        CORE_LOG_INFO("Creating window: {} ({}, {})", props.title, props.width, props.height);

        // Window hints
        glfwWindowHint(GLFW_DECORATED, true);
        glfwWindowHint(GLFW_RESIZABLE, props.resizable);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are using modern OpenGL
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // Remove functionality deprecated in the requested OpenGL version

#if defined(FENIX_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        m_Window = glfwCreateWindow(static_cast<int>(props.width),
                                    static_cast<int>(props.height),
                                    m_Data.Title.c_str(),
                                    nullptr,
                                    nullptr);

        if (props.maximized)
            glfwMaximizeWindow(m_Window);

        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        ++s_GLFWWindowCount;
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Initialize input
        Input::Init();

        // Set GLFW event callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Width       = width;
            data.Height      = height;
            WindowResizeEvent event(width, height); // Construct the appropriate event object
            data.EventCallback(event); // And pass it to the window's event callback function
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        // clang-format off
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });
        // clang-format on

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key_code) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(key_code);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(
            m_Window,
            [](GLFWwindow* window, int button, int action, int /* mods */) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                switch (action)
                {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_offset, double y_offset) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x_pos, double y_pos) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(x_pos), static_cast<float>(y_pos));
            data.EventCallback(event);
        });
    }

    void LinuxWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;
    }

    void LinuxWindow::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void LinuxWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval((enabled) ? 1 : 0);
        m_Data.VSync = enabled;
    }

    void LinuxWindow::SetViewportSize(u32 width, u32 height)
    {
        glViewport(0, 0, width, height);
    }

    bool LinuxWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

} // namespace fenix
