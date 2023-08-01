#pragma once

#include "fenix/renderer/render_command.hh"

namespace fenix {

    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertex_array);
    };

} // namespace fenix
