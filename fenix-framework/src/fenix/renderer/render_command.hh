#pragma once

#include <glm/glm.hpp>
#include "fenix/renderer/buffer.hh"
#include "fenix/utils/colors.hh"

namespace fenix {

    class RenderCommand
    {
    public:
        static void SetViewport(u32 x, u32 y, u32 width, u32 height);
        static void SetClearColor(Color color);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array);
        static void DrawIndexed(u32 num_indices);
    };

} // namespace fenix
