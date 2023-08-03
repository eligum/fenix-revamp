#include "fenix/renderer/camera.hh"
#include "fenix/input/input.hh"
#include "fenix/core/base.hh"

FX_DISABLE_WARNING_PUSH
FX_DISABLE_WARNING_USELESS_CAST
#include <glm/ext.hpp>
FX_DISABLE_WARNING_POP

namespace fenix {

    ///////////////////////////////////////////////////////////////////////////
    // Editor Camera //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    EditorCamera::EditorCamera(f32 xz_angle,
                               f32 xy_angle,
                               f32 distance,
                               glm::vec3 center,
                               glm::vec3 up,
                               f32 fov,
                               f32 aspect_ratio,
                               f32 z_near,
                               f32 z_far)
        : m_Fov(fov), m_AspectRatio(aspect_ratio), m_NearClip(z_near), m_FarClip(z_far),
          m_XZ_angle(glm::radians(xz_angle)), m_XY_angle(glm::radians(xy_angle)),
          m_Distance(distance), m_Center(center), m_Up(up)
    {
        update_projection_matrix();
        update_view_matrix();
    }

    void EditorCamera::update_projection_matrix()
    {
        m_Projection = glm::perspectiveRH(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::update_view_matrix()
    {
        using namespace glm;

        vec3 V, position;

        V.x = cos(m_XZ_angle) * cos(m_XY_angle);
        V.y = sin(m_XY_angle);
        V.z = sin(m_XZ_angle) * cos(m_XY_angle);

        position = m_Distance * normalize(V);

        m_View = glm::lookAtRH(position, m_Center, m_Up);
    }

    void EditorCamera::OnUpdate(TimeStep /* ts */)
    {
        if (Input::IsKeyPressed(Key::LeftAlt))
        {
            auto mouse_pos = Input::GetCursorPosition();
            auto delta = mouse_pos - m_PreviousMousePosition;
            m_PreviousMousePosition = mouse_pos;

            if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
            {
                rotate_camera(delta);
            }
            else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
            {
                // pan_camera(delta);
            }
        }

        if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
        {
            m_PreviousMousePosition = Input::GetCursorPosition();
        }
    }

    void EditorCamera::rotate_camera(const glm::vec2& delta)
    {
        const f32 factor = 0.05f * def::EDITOR_CAMERA_ROTATION_SPEED;
        m_XZ_angle += delta.x * factor;
        m_XY_angle += delta.y * factor;
        m_XY_angle = glm::clamp(m_XY_angle, glm::radians(-89.9f), glm::radians(89.9f));

        update_view_matrix();
    }

} // namespace fenix
