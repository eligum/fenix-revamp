#pragma once

#include <glm/vec2.hpp>
#include "fenix/input/key_codes.hh"
#include "fenix/input/mouse_codes.hh"

// TODO(Miguel): Add key and mouse button release action detection.

namespace fenix::Input {

    /// Initializes input.
    void Init();

    /// Polls the last reported per-window state of a specific key and returns true if the
    /// key was pressed. State is updated every frame.
    bool IsKeyPressed(KeyCode key);

    /// Polls the last reported per-window state of a specific mouse button and returns true
    /// if the mouse button was pressed. State is updated every frame.
    bool IsMouseButtonPressed(MouseCode button);

    /// Returns true if the cursor is hovering the application's main window.
    bool IsCursorInsideWindow();

    /// Returns the positon of the cursor, in screen coordinates, relative to the upper-left
    /// corner of the content area of the aplication's main window.
    glm::vec2 GetCursorPosition();

    /// Sets the mouse motion input mode for the application's main window.
    void SetCursorInputMode(CursorMode mode);

} // namespace fenix::Input
