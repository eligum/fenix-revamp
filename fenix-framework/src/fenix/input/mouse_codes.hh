#pragma once

#include "fenix/utils/std_types.hh"

namespace fenix {

    using MouseCode = i16;

    enum class CursorMode
    {
        Normal   = 0x00034001, // GLFW_CURSOR_NORMAL
        Hidden   = 0x00034002, // GLFW_CURSOR_HIDDEN,
        Disabled = 0x00034003, // GLFW_CURSOR_DISABLED,
    };

    // NOTE(Miguel): By not using an enum class we avoid having to cast to an int when we need
    // to retrieve the mouse code.

    enum Mouse : MouseCode
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

} // namespace fenix
