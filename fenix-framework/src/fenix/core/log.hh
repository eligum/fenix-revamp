#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace fenix {

    class Log
    {
    public:
        static void Init();

        static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

} // namespace fenix

// Framework log macros
#define CORE_LOG_TRACE(...) ::fenix::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)  ::fenix::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)  ::fenix::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...) ::fenix::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_FATAL(...) ::fenix::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Clien log macros
#define LOG_TRACE(...) ::fenix::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::fenix::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::fenix::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::fenix::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) ::fenix::Log::GetClientLogger()->critical(__VA_ARGS__)
