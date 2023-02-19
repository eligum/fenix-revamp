#include "fenix/utils/random.hh"

namespace fenix {

    // Static variables declared in a class need to live somewhere so we define them here.
    // The variable `s_Engine` will get properly initialized once `Random::Init()` is called.
    // Any attempt to access or modify this variable from outside the Random class will result
    // in a compilation error since it has been declared private.
    std::mt19937 Random::s_Engine;

} // namespace fenix
