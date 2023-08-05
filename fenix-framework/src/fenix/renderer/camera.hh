#pragma once

#include <glm/glm.hpp>
#include "fenix/events/mouse_event.hh"
#include "fenix/renderer/bounding_box.hh"
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

    /// A camera class capable of rotation around a central point, with the point
    /// of rotation movable via panning.
    class EditorCamera : public Camera
    {
    public:
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

        void OnUpdate(TimeStep ts);
        void OnEvent(Event& event);

        auto GetViewMatrix() const -> const glm::mat4& { return m_View; }
        auto GetProjectionViewMatrix() const -> glm::mat4 { return m_Projection * m_View; }
        f32 GetDistance() const { return m_Distance; }

        auto GetPosition() const -> const glm::vec3& { return m_Position; }
        auto GetUpDirection() const -> glm::vec3;
        auto GetRightDirection() const -> glm::vec3;
        auto GetForwardDirection() const -> glm::vec3;

        void SetDistance(f32 distance) { m_Distance = distance; }
        void SetViewportSize(f32 width, f32 height);

        /// Adjusts the camera parameters so that the view frustum clips exactly around
        /// the perimeter of the given bounding box. It does so by setting the camera's
        /// focus (central point) to the center of the bounding box and moving the camera
        /// closer or farther without changing the current fov value.
        void FitToBox(const BoundingBox& box);

    private:
        void update_projection_matrix();
        void update_view_matrix();

        void rotate_camera(const glm::vec2& delta);
        void pan_camera(const glm::vec2& delta);
        void zoom_camera(f32 delta);
        auto calculate_position() const -> glm::vec3;
        auto calculate_pan_speed() const -> std::tuple<f32, f32>;
        f32 calculate_zoom_speed() const;

        bool on_mouse_scroll(const MouseScrolledEvent& event);

    private:
        f32 m_Fov;
        f32 m_AspectRatio;
        f32 m_NearClip;
        f32 m_FarClip;

        f32 m_XZ_angle;
        f32 m_XY_angle;
        f32 m_Distance;
        glm::vec3 m_FocalPoint;
        glm::vec3 m_Up;
        glm::vec3 m_Position;
        glm::mat4 m_View;

        f32 m_ViewportWidth;
        f32 m_ViewportHeight;
        glm::vec2 m_PreviousMousePosition;
    };

} // namespace fenix
