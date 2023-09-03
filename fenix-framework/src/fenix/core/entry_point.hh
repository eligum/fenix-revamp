#pragma once

#include "fenix/core/base.hh"
#include "fenix/core/log.hh"

extern fenix::Application* fenix::CreateApplication(CommandLineArgs args);

int main(int argc, char** argv)
{
    fenix::Log::Init();

    FENIX_PROFILE_BEGIN_SESSION("Startup", "startup.json");
    auto app = fenix::CreateApplication({ argc, argv });
    FENIX_PROFILE_END_SESSION();

    FENIX_PROFILE_BEGIN_SESSION("Runtime", "runtime.json");
    app->Run();
    FENIX_PROFILE_END_SESSION();

    FENIX_PROFILE_BEGIN_SESSION("Shutdown", "shutdown.json");
    delete app;
    FENIX_PROFILE_END_SESSION();
}
