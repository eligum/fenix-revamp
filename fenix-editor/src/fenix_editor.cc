#include <fenix.hh>
// #include <fenix/core/entry_point.hh>

int main(int argc, char** argv)
{
    fenix::Log::Init();

    auto app = fenix::CreateApplication({argc, argv});
    app->Run();
    delete app;
}

namespace fenix {

    Application* CreateApplication(CommandLineArgs args)
    {
        auto spec = ApplicationSpecification{};
        spec.start_maximized = true;
        spec.working_directory = ".";
        spec.command_line_args = args;

        return new Application(spec);
    }

} // namespace fenix
