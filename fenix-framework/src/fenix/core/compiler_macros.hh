#pragma once

// Compiler warnings
#if defined(_MSC_VER)
    #define FENIX_DISABLE_WARNING_PUSH            __pragma(warning( push ))
    #define FENIX_DISABLE_WARNING_POP             __pragma(warning( pop ))
    #define FENIX_DISABLE_WARNING(warning_number) __pragma(warning( disable : warning_number ))

    #define FENIX_DISABLE_WARNING_USELESS_CAST
    #define FENIX_DISABLE_WARNING_VARIABLE_SHADOWING

#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(x) _Pragma(#x)
    #define FENIX_DISABLE_WARNING_PUSH          DO_PRAGMA(GCC diagnostic push)
    #define FENIX_DISABLE_WARNING_POP           DO_PRAGMA(GCC diagnostic pop)
    #define FENIX_DISABLE_WARNING(warning_name) DO_PRAGMA(GCC diagnostic ignored #warning_name)

    #define FENIX_DISABLE_WARNING_USELESS_CAST       FENIX_DISABLE_WARNING(-Wuseless-cast)
    #define FENIX_DISABLE_WARNING_VARIABLE_SHADOWING FENIX_DISABLE_WARNING(-Wshadow)
#endif

// Debug break points
#if defined(_MSC_VER)
    #define FENIX_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <csignal>
    #if defined(SIGTRAP)
        #define FENIX_DEBUGBREAK() std::raise(SIGTRAP)
    #else
        #define FENIX_DEBUGBREAK() std::raise(SIGABRT)
    #endif
#endif

// Function signatures
#if defined(_MSC_VER)
    #define FENIX_FUNCTION_SIG __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
    #define FENIX_FUNCTION_SIG __PRETTY_FUNCTION__
#endif
