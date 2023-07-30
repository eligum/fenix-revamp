#pragma once

#include <cmath>
#include <filesystem>
#include "fenix/core/window.hh"
#include "fenix/events/application_event.hh"
#include "fenix/core/layer_stack.hh"
#include "fenix/utils/std_types.hh"

namespace fenix {

    /// Wrapper for the command line arguments passed to the application that overloads the
    /// `[]` operator giving direct acces to the arguments with bounds checking.
    struct CommandLineArgs
    {
        i32 count = 0;
        char** arguments = nullptr;

        /// Accepts negative index values.
        const char* operator[](i32 index) const
        {
            FENIX_ASSERT(std::abs(index) < count, "Index out of bounds.");
            return (index >= 0) ? arguments[index] : arguments[count + index];
        }
    };

    struct ApplicationSpecification
    {
        std::filesystem::path working_directory;
        bool start_maximized;
        CommandLineArgs command_line_args;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& spec);
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void Close();

        // Disable copy/move constructors and assignment/move operators
        Application(Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&) = delete;
        Application& operator=(Application&&) = delete;

        /// Returns a handle to the application's main window.
        Window& GetWindow() { return *m_MainWindow; }

        static Application& GetInstance() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent&);
        bool OnWindowResize(WindowResizeEvent& event);

    private:
        using Clock = std::chrono::steady_clock;
        using Seconds = std::chrono::duration<f32>;

    private:
        ApplicationSpecification m_Specification;
        std::unique_ptr<Window> m_MainWindow;
        bool m_Running;
        bool m_Minimized; // Only needed for Windows OS
        Clock::time_point m_LastFrameTime;

        LayerStack m_LayerStack;

    private:
        static Application* s_Instance;
    };

    /// To be defined in client.
    Application* CreateApplication(CommandLineArgs args);

} // namespace fenix
