#pragma once

#include "fenix/core/window.hh"
#include "fenix/backend/opengl_context.hh"

struct GLFWwindow; // Forward declaration to avoid having to include GLFW here

namespace fenix {


    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProps& props);
        ~LinuxWindow();

        void OnUpdate() override;

        u32 GetWidth() const override { return m_Data.Width; }
        u32 GetHeight() const override { return m_Data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        void SetViewportSize(u32 width, u32 height) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        GLFWwindow* m_Window;
        OpenGLContext* m_Context;

        struct WindowData {
            std::string Title;
            u32 Width;
            u32 Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // namespace fenix
