#pragma once

#include <memory>
#include "fenix/core/compiler_macros.hh"

#ifdef FENIX_DEBUG
    #define FENIX_ENABLE_ASSERTS
    #define FENIX_ENABLE_PROFILING
#endif

#ifdef FENIX_ENABLE_ASSERTS
    #define FENIX_ASSERT(x, ...)                                                          \
        {                                                                                 \
            if (!(x))                                                                     \
            {                                                                             \
                CORE_LOG_ERROR("Assertion failed at LINE {} in {}:", __LINE__, __FILE__); \
                CORE_LOG_ERROR("{}", __VA_ARGS__);                                        \
                FENIX_DEBUGBREAK();                                                       \
            }                                                                             \
        }
#else
    #define FENIX_ASSERT(x, ...)
#endif

#define FENIX_STRINGIFY(x) #x

#define BIT(x) (1 << x)

#define FENIX_BIND_EVENT_FN(fn)                                 \
    [this](auto&&... args) -> decltype(auto) {                  \
        return this->fn(std::forward<decltype(args)>(args)...); \
    }

namespace fenix {

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename ...Args>
    constexpr auto CreateScope(Args&&... args) -> Scope<T>
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // A thread-safe reference-counting pointer.
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ...Args>
    constexpr auto CreateRef(Args&&... args) -> Ref<T>
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using WeakRef = std::weak_ptr<T>;

}

#include "fenix/core/log.hh"
#include "fenix/utils/std_types.hh"
#include "fenix/utils/profiler.hh"
