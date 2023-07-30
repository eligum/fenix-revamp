#include <filesystem>
#include <chrono>
#include "fenix/core/application.hh"
#include "fenix/core/log.hh"

#include <GLFW/glfw3.h>

namespace fenix {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& spec)
        : m_Specification(spec),
          m_MainWindow(nullptr),
          m_Running(false),
          m_Minimized(false),
          m_LastFrameTime(Clock::now())
    {
        FENIX_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        if (!m_Specification.working_directory.empty())
        {
            std::filesystem::current_path(m_Specification.working_directory);
        }

        auto wp = WindowProps {
            "FENIX Engine",
            1280,
            720,
            true,
            m_Specification.start_maximized
        };

        m_MainWindow = std::unique_ptr<Window> { Window::Create(wp) };
        m_MainWindow->SetEventCallback(FENIX_BIND_EVENT_FN(Application::OnEvent));
        m_MainWindow->SetVSync(true);
    }

    Application::~Application()
    {
        s_Instance = nullptr;
    }

    void Application::OnEvent(Event& evt)
    {
        EventDispatcher dispatcher(evt);
        dispatcher.Dispatch<WindowCloseEvent>(FENIX_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(FENIX_BIND_EVENT_FN(Application::OnWindowResize));

        // Send the event to all layers of the stack in order until one of them handles it.
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (evt.handled) { break; }
            (*it)->OnEvent(evt);
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::Run()
    {
        m_Running = true;

        while (m_Running)
        {
            auto time       = Clock::now();
            auto delta_time = TimeStep { Seconds { time - m_LastFrameTime }.count() };
            m_LastFrameTime = time;

            // In Windows OS minimizing causes the screen to get resized to 0,0 while still
            // consuming resources, to avoid wasting CPU or GPU time we only render when the
            // window size is greater than zero.
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(delta_time);
                }
            }

            m_MainWindow->OnUpdate();
        }
    }

    void Application::Close()
    {
        m_Running = false;
    }

    bool Application::OnWindowClose(WindowCloseEvent&)
    {
        Close();
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& evt)
    {
        if (evt.GetWidth() == 0 || evt.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        // m_MainWindow->SetViewportSize(evt.GetWidth(), evt.GetHeight());
        // Renderer::OnWindowResize(evt.GetWidth(), evt.GetHeight());

        return false;
    }

} // namespace fenix
