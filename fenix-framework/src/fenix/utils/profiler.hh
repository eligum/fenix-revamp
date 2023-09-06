#pragma once

#include <filesystem>
#include <chrono>
#include <ratio>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include "fenix/utils/singleton.hh"
#include "fenix/core/log.hh"

namespace fenix {

    struct ProfileResults
    {
        std::string_view Name;
        std::chrono::steady_clock::time_point Start;
        std::chrono::duration<double, std::micro> ElapsedTime;
        std::thread::id ThreadID;
    };

    struct ProfilerSession
    {
        std::string_view Name;
        ProfilerSession(std::string_view name) : Name(name) {}
    };

    class Profiler : public Singleton<Profiler>
    {
    public:
        /// Nested timer class.
        /// It only makes sense within the context of a Profiler.
        class Timer
        {
        public:
            using clock        = std::chrono::steady_clock;
            using microseconds = std::chrono::duration<double, std::micro>;

        public:
            explicit Timer(std::string_view name)
                : m_name(name), m_stopped(false)
            {
                m_start = clock::now();
            }

            ~Timer()
            {
                if (!m_stopped)
                    Stop();
            }

            void Stop()
            {
                auto end          = clock::now();
                auto elapsed_time = microseconds { end - m_start };

                Profiler::GetInstance().WriteProfile({
                    m_name,
                    m_start,
                    elapsed_time,
                    std::this_thread::get_id(),
                });

                m_stopped = true;
            }

        private:
            std::string_view m_name;
            clock::time_point m_start;
            bool m_stopped;
        };

    public:
        void BeginSession(std::string_view name, const std::filesystem::path& file = "results.json")
        {
            auto lock = std::lock_guard{m_mutex};
            if (!m_current_session)
            {
                m_output_stream.open(file);
                if (m_output_stream.is_open())
                {
                    m_current_session = std::make_unique<ProfilerSession>(name);
                    WriteHeader();
                }
            }
        }

        void EndSession()
        {
            auto lock = std::lock_guard{m_mutex};
            if (m_current_session)
            {
                WriteFooter();
                m_output_stream.close();
                m_current_session.reset();
            }
        }

        void WriteProfile(const ProfileResults& results)
        {
            std::stringstream json;

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << R"("cat":"function",)";
            json << R"("dur":)" << results.ElapsedTime.count() << ',';
            json << R"("name":")" << results.Name << R"(",)";
            json << R"("ph":"X",)";
            json << R"("pid":0,)";
            json << R"("tid":)" << results.ThreadID << ",";
            json << R"("ts":)" << Timer::microseconds{ results.Start.time_since_epoch() }.count();
            json << "}";

            auto lock = std::lock_guard{m_mutex};
            if (m_current_session)
            {
                m_output_stream << json.str();
                m_output_stream.flush();
            }
        }

    protected:
        Profiler() = default;

        void WriteHeader()
        {
            m_output_stream << R"({"other_data":{},"trace_events":[{})";
            m_output_stream.flush();
        }

        void WriteFooter()
        {
            m_output_stream << "]}";
            m_output_stream.flush();
        }

    private:
        std::mutex m_mutex;
        std::unique_ptr<ProfilerSession> m_current_session;
        std::ofstream m_output_stream;
    };

} // namespace fenix
