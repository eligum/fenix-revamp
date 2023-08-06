#include "fenix/renderer/camera.hh"
#include "fenix/input/input.hh"
#include "fenix/core/base.hh"

FX_DISABLE_WARNING_PUSH
FX_DISABLE_WARNING_USELESS_CAST
#include <glm/ext.hpp>
#include <glm/trigonometric.hpp>
FX_DISABLE_WARNING_POP

namespace fenix {

    ///////////////////////////////////////////////////////////////////////////
    // Editor Camera //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    // TODO(Miguel): Implement rotations using quaternions

    EditorCamera::EditorCamera(f32 xz_angle,
                               f32 xy_angle,
                               f32 distance,
                               glm::vec3 center,
                               glm::vec3 up,
                               f32 fov,
                               f32 aspect_ratio,
                               f32 z_near,
                               f32 z_far)
        : m_Fov(glm::radians(fov)), m_AspectRatio(aspect_ratio), m_NearClip(z_near), m_FarClip(z_far),
          m_XZ_angle(glm::radians(xz_angle)), m_XY_angle(glm::radians(xy_angle)),
          m_Distance(distance), m_FocalPoint(center), m_Up(up)
    {
        update_projection_matrix();
        update_view_matrix();
    }

    void EditorCamera::OnEvent(Event& event)
    {
        auto dispacher = EventDispatcher{event};
        dispacher.Dispatch<MouseScrolledEvent>(FENIX_BIND_EVENT_FN(EditorCamera::on_mouse_scroll));
    }

    void EditorCamera::OnUpdate(TimeStep ts)
    {
        if (Input::IsKeyPressed(Key::LeftAlt))
        {
            auto mouse_pos = Input::GetCursorPosition();
            auto delta = mouse_pos - m_LastMousePosition;
            m_LastMousePosition = mouse_pos;

            if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
            {
                rotate_camera(delta);
            }
            else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
            {
                pan_camera(delta);
            }
        }

        if (Input::IsKeyPressed(Key::Up))
        {
            m_FocalPoint += 2.0f * ts * GetUpDirection();
            update_view_matrix();
        }
        if (Input::IsKeyPressed(Key::Left))
        {
            m_FocalPoint -= 2.0f * ts * GetRightDirection();
            update_view_matrix();
        }
        if (Input::IsKeyPressed(Key::Right))
        {
            m_FocalPoint += 2.0f * ts * GetRightDirection();
            update_view_matrix();
        }
        if (Input::IsKeyPressed(Key::Down))
        {
            m_FocalPoint -= 2.0f * ts * GetUpDirection();
            update_view_matrix();
        }

        if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
        {
            m_LastMousePosition = Input::GetCursorPosition();
        }
    }

    auto EditorCamera::calculate_position() const -> glm::vec3
    {
        using namespace glm;

        vec3 V = {
             cos(m_XZ_angle) * cos(m_XY_angle),
             sin(m_XY_angle),
            -sin(m_XZ_angle) * cos(m_XY_angle),
        };

        return m_FocalPoint + m_Distance * normalize(V);
    }

    void EditorCamera::update_projection_matrix()
    {
        m_Projection = glm::perspectiveRH(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::update_view_matrix()
    {
        m_Position = calculate_position();
        m_View = glm::lookAtRH(m_Position, m_FocalPoint, m_Up);
    }

    bool EditorCamera::on_mouse_scroll(const MouseScrolledEvent& event)
    {
        f32 offset = event.GetYOffset();
        zoom_camera(offset);
        return false;
    }

    void EditorCamera::rotate_camera(const glm::vec2& delta)
    {
        const f32 factor = 0.05f * def::EDITOR_CAMERA_ROTATION_SPEED;
        m_XZ_angle -= delta.x * factor;
        m_XY_angle += delta.y * factor;
        m_XY_angle = glm::clamp(m_XY_angle, glm::radians(-89.9f), glm::radians(89.9f));
        update_view_matrix();
    }

    void EditorCamera::pan_camera(const glm::vec2& delta)
    {
        auto [x_factor, y_factor] = calculate_pan_speed();
        m_FocalPoint += -GetRightDirection() * x_factor * delta.x;
        m_FocalPoint += GetUpDirection() * y_factor * delta.y;
        update_view_matrix();
    }

    // TODO: Use a less rudimentary zoom system
    void EditorCamera::zoom_camera(f32 delta)
    {
        f32 factor = (delta > 0) ? 0.8 : (1 / 0.8);
        m_Distance = std::clamp(factor * m_Distance, 1.0f, 1009.742f);
        // LOG_TRACE("After zoom:");
        // LOG_TRACE("  delta {}", delta);
        // LOG_TRACE("  distance {}", m_Distance);
        update_view_matrix();
    }

    auto EditorCamera::calculate_pan_speed() const -> std::tuple<f32, f32>
    {
        f32 x_factor = 0.01f * def::EDITOR_CAMERA_PAN_SPEED * m_Distance;
        f32 y_factor = 0.01f * def::EDITOR_CAMERA_PAN_SPEED * m_Distance;
        return {x_factor, y_factor};
    }

    f32 EditorCamera::calculate_zoom_speed() const
    {
        f32 factor = 0.2f * def::EDITOR_CAMERA_ZOOM_SPEED;
        f32 speed = std::min(factor * m_Distance * m_Distance, 100.0f);
        return speed;
    }

    void EditorCamera::FitToBox(const BoundingBox& box)
    {
        auto new_center = box.ComputeCenter();
        f32 radius = glm::length(box.GetMaxPoint() - new_center);
        f32 alpha = glm::radians(m_Fov / 2.0f);
        f32 distance = radius * glm::sin(alpha);

        m_FocalPoint = new_center;
        m_Distance = distance;
        m_NearClip = distance - radius;
        m_FarClip = distance + radius;
        update_view_matrix();
    }

    void EditorCamera::SetViewportSize(f32 width, f32 height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        m_AspectRatio = height / width;
        update_projection_matrix();
    }

    auto EditorCamera::GetUpDirection() const -> glm::vec3
    {
        return glm::normalize(glm::cross(GetRightDirection(), GetForwardDirection()));
    }

    auto EditorCamera::GetRightDirection() const -> glm::vec3
    {
        return glm::normalize(glm::cross(m_FocalPoint - m_Position, m_Up));
    }

    auto EditorCamera::GetForwardDirection() const -> glm::vec3
    {
        return glm::normalize(m_FocalPoint - m_Position);
    }

} // namespace fenix
