#include "fenix/core/application.hh"

#include <GLFW/glfw3.h>
#include "fenix/core/log.hh"

namespace fenix {

static void create_glfw_window(const WindowProps& wp);

Application::Application(const WindowProps& wp)
{
    create_glfw_window(wp);
}

void Application::Init()
{
}

////////////////////////////////////////////////////////////////////////////////
/// Static free functions definitions //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void glfw_error_callback(int error, const char* description)
{
    CORE_LOG_ERROR("GLFW ({}): {}", error, description);
}

static void create_glfw_window(const WindowProps& wp)
{
    i32 success = glfwInit();
    if (!success)
    {
        CORE_LOG_FATAL("Failed to initialize GLFW!");
    }
    glfwSetErrorCallback(glfw_error_callback);

    CORE_LOG_INFO("Creating window: {} ({}, {})", wp.Title, wp.Width, wp.Height);

    // Set the window minimum opengl context version required
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are using modern OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // Set to false if it causes trouble

    if (!wp.Resizable)
        glfwWindowHint(GLFW_RESIZABLE, false);
}

} // namespace fenix