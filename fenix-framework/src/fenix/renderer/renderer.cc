#include "fenix/renderer/renderer.hh"

namespace fenix {

    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertex_array)
    {
        vertex_array->Bind();
        RenderCommand::DrawIndexed(vertex_array);
        vertex_array->Unbind();
    }

} // namespace fenix