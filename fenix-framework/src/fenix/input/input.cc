#include "fenix/input/input.hh"
#include "fenix/core/application.hh"

namespace fenix::Input {

    static bool s_RawMouseMotion = false;

    void Init()
    {
        s_RawMouseMotion = glfwRawMouseMotionSupported();
    }

    bool IsKeyPressed(KeyCode key)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }

    bool IsMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    bool IsCursorInsideWindow()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        return glfwGetWindowAttrib(window, GLFW_HOVERED);
    }

    glm::vec2 GetCursorPosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        f64 x_pos;
        f64 y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return glm::vec2{x_pos, y_pos};
    }

    void SetCursorInputMode(CursorMode mode)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        glfwSetInputMode(window, GLFW_CURSOR, static_cast<int>(mode));

        if (mode == CursorMode::Disabled && s_RawMouseMotion) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, true);
        }
    }

} // namespace fenix::Input

/*

`glfwPollEvents()`
Processes only those events that have already been received and then returns immediately.

`glfwWaitEvents()`
Puts the thread to sleep until one event has been received and then processes all received events.

`glfwWaitEventsTimeout()`
Puts the thread to sleep until at least one event has been received, or until the specified number
of seconds have elapsed.

*/
