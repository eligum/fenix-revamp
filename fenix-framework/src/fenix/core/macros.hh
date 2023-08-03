#pragma once

#if defined(_MSC_VER)
    #define FX_DISABLE_WARNING_PUSH            __pragma(warning( push ))
    #define FX_DISABLE_WARNING_POP             __pragma(warning( pop ))
    #define FX_DISABLE_WARNING(warning_number) __pragma(warning( disable : warning_number ))

    #define FX_DISABLE_WARNING_USELESS_CAST
    #define FX_DISABLE_WARNING_VARIABLE_SHADOWING

#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(x) _Pragma(#x)
    #define FX_DISABLE_WARNING_PUSH          DO_PRAGMA(GCC diagnostic push)
    #define FX_DISABLE_WARNING_POP           DO_PRAGMA(GCC diagnostic pop)
    #define FX_DISABLE_WARNING(warning_name) DO_PRAGMA(GCC diagnostic ignored #warning_name)

    #define FX_DISABLE_WARNING_USELESS_CAST       FX_DISABLE_WARNING(-Wuseless-cast)
    #define FX_DISABLE_WARNING_VARIABLE_SHADOWING FX_DISABLE_WARNING(-Wshadow)
#endif
