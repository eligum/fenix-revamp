#pragma once

#include "fenix/utils/std_types.hh"
#include <GLFW/glfw3.h>

namespace fenix {

    using MouseCode = i16;

    enum class CursorMode
    {
        Normal   = GLFW_CURSOR_NORMAL,
        Hidden   = GLFW_CURSOR_HIDDEN,
        Disabled = GLFW_CURSOR_DISABLED,
    };

    // By putting a plain enum inside a namespace instead of using an enum class we avoid having
    // to cast to an int when we need to retrieve the mouse code.
    namespace Mouse {
        enum : MouseCode
        {
            Button0      = 0,
            Button1      = 1,
            Button2      = 2,
            Button3      = 3,
            Button4      = 4,
            Button5      = 5,
            Button6      = 6,
            Button7      = 7,

            ButtonLast   = Button7,
            ButtonLeft   = Button0,
            ButtonRight  = Button1,
            ButtonMiddle = Button2,
        };
    }

} // namespace fenix
