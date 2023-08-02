#pragma once

#include <glm/glm.hpp>

namespace fenix {

    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection) : m_Projection(projection) {}
        virtual ~Camera() = default;

        auto GetProjection() const -> const glm::mat4& { return m_Projection; }

    protected:
        glm::mat4 m_Projection;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Editor Camera //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    class EditorCamera : public Camera
    {
    public:
        EditorCamera();
    private:
    };

} // namespace fenix
