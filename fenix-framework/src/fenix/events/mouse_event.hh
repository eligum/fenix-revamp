#pragma once

#include <sstream>

#include "event.hh"
#include "fenix/input/mouse_codes.hh"

namespace fenix {

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(f32 x, f32 y) : m_MouseX(x), m_MouseY(y) {}

        f32 GetX() const { return m_MouseX; }
        f32 GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        f32 m_MouseX;
        f32 m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const f32 xOffset, const f32 yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        f32 GetXOffset() const { return m_XOffset; }
        f32 GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        f32 m_XOffset;
        f32 m_YOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        MouseButtonEvent(MouseCode button) : m_Button(button) {}

        MouseCode m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

} // namespace fenix
