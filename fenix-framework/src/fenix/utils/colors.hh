#pragma once

#include <glm/vec4.hpp>

namespace fenix {

    enum class Color
    {
        Black,
        White,
        Red,
        Orange,
        Yellow,
        Chartreuse,
        Green,
        Aquamarine,
        Cyan,
        Azure,
        Blue,
        Violet,
        Magenta,
        Rose,
    };

    inline auto GetColorRGBA(Color color) -> glm::vec4
    {
        switch (color)
        {
            case Color::Black:      return { 0.0f, 0.0f, 0.0f, 1.0f };
            case Color::White:      return { 1.0f, 1.0f, 1.0f, 1.0f };
            case Color::Red:        return { 1.0f, 0.0f, 0.0f, 1.0f };
            case Color::Orange:     return { 1.0f, 0.5f, 0.0f, 1.0f };
            case Color::Yellow:     return { 1.0f, 1.0f, 0.0f, 1.0f };
            case Color::Chartreuse: return { 0.5f, 1.0f, 0.0f, 1.0f };
            case Color::Green:      return { 0.0f, 1.0f, 0.0f, 1.0f };
            case Color::Aquamarine: return { 0.0f, 1.0f, 0.5f, 1.0f };
            case Color::Cyan:       return { 0.0f, 1.0f, 1.0f, 1.0f };
            case Color::Azure:      return { 0.0f, 0.5f, 1.0f, 1.0f };
            case Color::Blue:       return { 0.0f, 0.0f, 1.0f, 1.0f };
            case Color::Violet:     return { 0.5f, 0.0f, 1.0f, 1.0f };
            case Color::Magenta:    return { 1.0f, 0.0f, 1.0f, 1.0f };
            case Color::Rose:       return { 1.0f, 0.0f, 0.5f, 1.0f };
        }

        return glm::vec4{ 1.0f };
    }

} // namespace fenix
