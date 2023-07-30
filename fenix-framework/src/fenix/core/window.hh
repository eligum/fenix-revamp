#pragma once

#include <string>
#include <functional>

#include "fenix/events/event.hh"

namespace fenix {

    struct WindowProps {
        std::string title;
        u32 width;
        u32 height;
        bool resizable;
        bool maximized;

        WindowProps(const std::string& title = "FENIX - Default Title",
                    u32 width = 1280,
                    u32 height = 720,
                    bool resizeable = true,
                    bool maximized = false)
            : title(title), width(width), height(height), resizable(resizeable),
              maximized(maximized) {}
    };

    /// Interface representing a desktop system based window.
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual u32 GetWidth() const = 0;
        virtual u32 GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual void SetViewportSize(u32 width, u32 height) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };

} // namespace fenix
