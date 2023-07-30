#pragma once

#include "fenix/core/log.hh"

extern fenix::Application* fenix::CreateApplication(CommandLineArgs args);

int main(int argc, char** argv)
{
    fenix::Log::Init();

    auto app = fenix::CreateApplication({argc, argv});
    app->Run();
    delete app;
}
