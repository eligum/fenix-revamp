#pragma once

#include <random>
#include <functional>
#include "fenix/utils/std_types.hh"

namespace fenix {

    /// Universally Unique Identifier (UUID).
    /// The internal value is a 64-bit integer, so it is virtually impossible
    /// to have collisions for tasks such as asset identification. The only
    /// situation where you might get collisions is if you are programmatically
    /// generating billions of UUIDs, and still the chances of encountering
    /// collisions in that situation are extremely low.
    class UUID
    {
    public:
        /// Default constructor. The resulting UUID is in an invalid state.
        UUID() : m_UUID(0) {}

        /// Construct a UUID from an unsigned 64-bit integer.
        UUID(u64 uuid) : m_UUID(uuid) {}

        /// Default copy constructor.
        UUID(const UUID&) = default;

        /// Allow UUIDs to be directly casted to an unsigned 64-bit integer.
        operator u64() const { return m_UUID; }

    private:
        u64 m_UUID;
    };

    /// Class for generating UUIDs.
    /// The underlying Random Number Generator (RNG) engine is NOT thread-safe,
    /// this class should be instantiated on every thread where you want to
    /// generate UUIDs.
    class UUIDGenerator
    {
    public:
        /// Default constructor. Initializes the RNG engine.
        UUIDGenerator() { m_Engine.seed(std::random_device {}()); }

        /// Returns a new UUID (Universally Unique Identifier).
        UUID generate() { return std::uniform_int_distribution<u64>{ 1, std::numeric_limits<u64>::max() }(m_Engine); }

    private:
        std::mt19937_64 m_Engine;
    };

} // namespace fenix

/// Custom template specialization of std::hash for UUID types to make them
/// hashable by STL containers.
template <>
struct std::hash<fenix::UUID>
{
    std::size_t operator()(const fenix::UUID& uuid) const noexcept
    {
        return std::hash<fenix::u64>{}(static_cast<fenix::u64>(uuid));
    }
};
