#pragma once

#include <glm/glm.hpp>
#include "fenix/events/mouse_event.hh"
#include "fenix/utils/std_types.hh"
#include "fenix/utils/definitions.hh"

namespace fenix {

    class Camera
    {
    public:
        Camera() : m_Projection(glm::mat4{ 1.0f }) {}
        Camera(const glm::mat4& projection) : m_Projection(projection) {}
        virtual ~Camera() = default;

        auto GetProjectionMatrix() const -> const glm::mat4& { return m_Projection; }

    protected:
        glm::mat4 m_Projection;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Editor Camera //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    class EditorCamera : public Camera
    {
    public:
        /// A camera that rotates around a certain point which can be moved through panning.
        /// This constructor expects angles to be in degrees.
        EditorCamera(f32 xz_angle     = def::EDITOR_CAMERA_XZ_ANGLE,
                     f32 xy_angle     = def::EDITOR_CAMERA_XY_ANGLE,
                     f32 distance     = def::EDITOR_CAMERA_DISTANCE,
                     glm::vec3 center = def::EDITOR_CAMERA_CENTER,
                     glm::vec3 up     = def::EDITOR_CAMERA_UP,
                     f32 fov          = def::EDITOR_CAMERA_FOV,
                     f32 aspect_ratio = def::EDITOR_CAMERA_ASPECT_RATIO,
                     f32 z_near       = def::EDITOR_CAMERA_Z_NEAR,
                     f32 z_far        = def::EDITOR_CAMERA_Z_FAR);

    public:
        void OnUpdate(TimeStep ts);
        void OnEvent(Event& event);

        auto GetViewMatrix() const -> const glm::mat4& { return m_View; }
        auto GetProjectionView() const -> glm::mat4 { return m_Projection * m_View; }

    private:
        void update_projection_matrix();
        void update_view_matrix();

        bool on_mouse_scrolled(const MouseScrolledEvent& event);

        void rotate_camera(const glm::vec2& delta);
        void pan_camera(const glm::vec2& delta);

    private:
        f32 m_Fov;
        f32 m_AspectRatio;
        f32 m_NearClip;
        f32 m_FarClip;

        f32 m_XZ_angle;
        f32 m_XY_angle;
        f32 m_Distance;
        glm::vec3 m_Center;
        glm::vec3 m_Up;

        glm::vec2 m_PreviousMousePosition;

        glm::mat4 m_View;
    };

} // namespace fenix
