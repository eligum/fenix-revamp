#pragma once

#include "fenix/core/base.hh"

namespace fenix {

    /// Helper class to create singletons.
    ///
    /// To understand the implementation of this class you will have to go down the C++ rabbit hole
    /// and do some research on the Curiously Recurring Template Pattern (CRTP) idiom.
    /// If you just want something that works, you are in luck because this class is very easy to use.
    ///
    /// @code{.cpp}
    /// class MyClass : public Singleton<MyClass> { ... };
    /// @endcode
    ///
    /// The previous snippet of code shows how to transform `MyClass` into a singleton, no more code
    /// is required, and it has 0 runtime overhead! Because everything is done at compile-time using
    /// template specialization.
    ///
    /// It has some limitations though, the class being singletonized cannot be marked as final and
    /// must have a public/protected default constructor. I recommend making the default constructor
    /// protected to prevent users of the class from instantiating it, it is a singleton after all.
    ///
    /// Remember that the decision to use a singleton should be well-justified based on the specific
    /// requirements of your application. Be cautious not to overuse singletons, as they will introduce
    /// global state and tight coupling between components, which can make maintaining and testing your
    /// code difficult.
    template <class C>
    class Singleton
    {
    public:
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&)      = delete;

        /// Returns a reference to the only instance of the `C` class.
        /// The instance is initialized the frist time this function is called. Class `C` must be
        /// default-constructible.
        static C& GetInstance()
        {
            if (!s_Instance)
                s_Instance = new C_;
            return *s_Instance;
        }

        /// Deletes the singleton instance.
        /// Class `C` will be initialized again the next time `GetInstance()` is called. You should
        /// never use this in normal code as you run into the risk of using a dangling reference.
        static void DeleteInstance()
        {
            if (s_Instance)
                delete s_Instance;
            s_Instance = nullptr;
        }

        /// Returns whether class `C` has been instanced.
        /// If this retruns false, all `C` references prior to this call are dangling and will cause
        /// undefined behaviour if used.
        static bool IsInstanced()
        {
            return s_Instance != nullptr;
        }

    protected:
        Singleton() {};

    private:
        // Trick to get around the fact that `C`'s constructor might be protected. A base class can't
        // access the protected members of its derived classes, so instead we instantiate a dummy class
        // that inherits from `C`. For this to work `C` can't be marked as final.
        struct C_ : public C
        {
            C_() : C() {}
        };

        static inline C* s_Instance = nullptr;
    };

} // namespace fenix
