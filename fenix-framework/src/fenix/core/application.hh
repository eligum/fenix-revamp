#pragma once

#include <string>
#include "fenix/utils/std_types.hh"

namespace fenix {

    struct WindowProps
    {
        std::string Title;
        u32 Width;
        u32 Height;
        bool Resizable;

        WindowProps(const std::string title = "FENIX - Application",
                    u32 width               = 1280,
                    u32 height              = 720,
                    bool resizable          = true)
            : Title(title), Width(width), Height(height), Resizable(resizable) {}
    };

    class Application
    {
    public:
        Application(const WindowProps& wp = {});
        virtual ~Application();

        void Init();
        void Run();
        void Shutdown();
    };

} // namespace fenix
