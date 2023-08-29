#include <string>
#include <iostream>

/// Function that only accepts string literals.
template <std::size_t N>
inline void OnlyStringLiterals(const char (&str)[N])
{
    std::cout << str << std::endl;
}

int main()
{
    const char* name = "This should NOT work";

    OnlyStringLiterals("This should work"); // Accepted
    OnlyStringLiterals(name); // Compilation error!
}
