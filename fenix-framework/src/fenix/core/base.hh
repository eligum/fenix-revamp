#pragma once

#include <memory>
#include <cstdlib>

#ifdef FENIX_DEBUG
#define FENIX_ENABLE_ASSERTS
#endif

#ifdef FENIX_ENABLE_ASSERTS
#define FENIX_ASSERT(x, ...)                                                          \
    {                                                                                 \
        if (!(x))                                                                     \
        {                                                                             \
            CORE_LOG_ERROR("Assertion failed at LINE {} in {}:", __LINE__, __FILE__); \
            CORE_LOG_ERROR("{}", __VA_ARGS__);                                        \
            std::exit(1);                                                             \
        }                                                                             \
    }
#else
#define FENIX_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FENIX_BIND_EVENT_FN(fn)                                 \
    [this](auto&&... args) -> decltype(auto) {                  \
        return this->fn(std::forward<decltype(args)>(args)...); \
    }

#include "fenix/core/macros.hh"
#include "fenix/core/log.hh"
#include "fenix/utils/std_types.hh"
