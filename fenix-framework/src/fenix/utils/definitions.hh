#pragma once

#include <glm/vec3.hpp>

namespace fenix::def {

    constexpr auto WINDOW_TITLE  = "FENIX - Engine";
    constexpr auto WINDOW_WIDTH  = 1280u;
    constexpr auto WINDOW_HEIGHT = 720u;

    constexpr auto EDITOR_CAMERA_XZ_ANGLE       = -45.0f;
    constexpr auto EDITOR_CAMERA_XY_ANGLE       = 30.0f;
    constexpr auto EDITOR_CAMERA_DISTANCE       = 4.0f;
    constexpr auto EDITOR_CAMERA_CENTER         = glm::vec3{0.0f, 0.0f, 0.0f};
    constexpr auto EDITOR_CAMERA_UP             = glm::vec3{0.0f, 1.0f, 0.0f};
    constexpr auto EDITOR_CAMERA_FOV            = 45.0f;
    constexpr auto EDITOR_CAMERA_ASPECT_RATIO   = 16.0f / 9.0f;
    constexpr auto EDITOR_CAMERA_Z_NEAR         = 0.1f;
    constexpr auto EDITOR_CAMERA_Z_FAR          = 100.0f;
    constexpr auto EDITOR_CAMERA_ROTATION_SPEED = 0.2f;
    constexpr auto EDITOR_CAMERA_ZOOM_SPEED     = 0.2f;

} // namespace fenix::def
