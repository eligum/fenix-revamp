#pragma once

#include <cstdint>
#include <chrono>


/// NOTE:
///
/// Define a set of types with fixed bit sizes, regardless of
/// platform in use. The typedefs specify the fixed types from
/// <cstdint> in a more convenient and intuitive manner.
///
/// It's advised to use if necessary, but sparringly. Using
/// the 'natural word size' of the machine is and will always
/// be faster for the future to come. So only use them when
/// doing fixed-size bit operations or having strict type
/// size requirements.
///
/// For instance, a loop variable doesn't need a fixed 32 bit
/// integer as the minimum integer requirement of 16 bits by
/// the C++ specification will suffice.

namespace fenix {

    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8  = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using f32 = float;
    using f64 = double;

    /// This class is simply a wrapper over a float.
    /// It uses the `<chrono>` library to make explicit which time units are being used upon
    /// construction. It can be implicitly casted to a float.
    class Seconds
    {
    public:
        explicit Seconds(f32 value = 0.0f)
            : m_Time(std::chrono::duration<f32>{value})
        {}

        operator f32() const { return m_Time.count(); }

    private:
        std::chrono::duration<f32> m_Time;
    };

} // namespace fenix
