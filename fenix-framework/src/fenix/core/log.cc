#include "fenix/core/log.hh"

#include <iostream>
#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace fenix {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        try
        {
            std::vector<spdlog::sink_ptr> sinks;
            sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
            sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_st>("fenix.log", true));

            sinks[0]->set_level(spdlog::level::trace);
            sinks[0]->set_pattern("%H:%M:%S %n [%^%l%$]: %v");
            sinks[1]->set_level(spdlog::level::trace);
            sinks[1]->set_pattern("[%H:%M:%S] [%n] [%l]: %v");

            s_CoreLogger = std::make_shared<spdlog::logger>("FENIX", sinks.begin(), sinks.end());
            spdlog::register_logger(s_CoreLogger);
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->flush_on(spdlog::level::trace);

            s_ClientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
            spdlog::register_logger(s_ClientLogger);
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cout << "Log initialization failed! Reason: " << ex.what() << std::endl;
        }
    }

} // namespace fenix
