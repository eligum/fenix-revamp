#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ratio>
#include "fenix/utils/std_types.hh"

// duration_cast<duration> -> truncates towards zero.
// floor<duration> -> truncates towards negative infinity.
// round<duration> -> truncates towards nearest and towards even on a tie.
// ceil<duration> -> truncates towards positive infinity.

namespace fenix {

    class Timer
    {
    public:
        /// Default constructor. Automatically starts the timer.
        Timer() { Reset(); }

        /// Resets the timer. This function can be called as many times as you want, only the
        /// most recent call will be considered as the timer's starting time point.
        void Reset() { m_tp0 = clock::now(); }

        /// Returns the elapsed time in seconds since the last call to `Reset()` as a float.
        [[nodiscard]] f32 Elapsed() { return seconds{clock::now() - m_tp0}.count(); }

        /// Returns the elapsed time in milliseconds since the last call to `Reset()` as an
        /// unsigned int. This function is meant for pretty debug output, if you need to operate
        /// with the returned value use `Elapsed()`.
        [[nodiscard]] u32 ElapsedMilli()
        {
            return std::chrono::round<milliseconds>(clock::now() - m_tp0).count();
        }

    private:
        using clock = std::chrono::steady_clock;
        using seconds = std::chrono::duration<f32>;
        using milliseconds = std::chrono::duration<u32, std::ratio<1, 1000>>;

    private:
        clock::time_point m_tp0;
    };

    /// Upon destruction, this timer writes to `stdout` the elapsed time in milliseconds since
    /// it was first created.
    class ScopedTimer
    {
    public:
        ScopedTimer(const std::string& name) : m_Name(name) {}
        ~ScopedTimer()
        {
            auto time = m_Timer.ElapsedMilli();
            std::cout << "[TIMER - " << m_Name << "]: " << time << "ms\n";
        }

    private:
        std::string m_Name;
        Timer m_Timer;
    };

} // namespace fenix
