#include "fenix/renderer/render_command.hh"
#include <glad/glad.h>

namespace fenix {

    void RenderCommand::SetViewport(u32 x, u32 y, u32 width, u32 height)
    {
        glViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(Color color)
    {
        glm::vec4 _color = GetColorRGBA(color);
        glClearColor(_color.r, _color.g, _color.b, _color.a);
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array)
    {
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void RenderCommand::DrawIndexed(u32 num_indices)
    {
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
    }

} // namespace fenix
