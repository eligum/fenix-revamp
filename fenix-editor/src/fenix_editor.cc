#include <fenix.hh>
#include <fenix/core/entry_point.hh>
#include <filesystem>

#include "editor_layer.hh"

namespace fs = std::filesystem;

namespace fenix {

    class FenixEditor : public Application
    {
    public:
        FenixEditor(const ApplicationSpecification& spec)
            : Application(spec)
        {
            this->PushLayer(new EditorLayer());
        }
    };

    Application* CreateApplication(CommandLineArgs args)
    {
        auto spec = ApplicationSpecification{};
        spec.start_maximized = true;
        spec.working_directory = fs::path(".");
        spec.command_line_args = args;

        return new FenixEditor(spec);
    }

} // namespace fenix
