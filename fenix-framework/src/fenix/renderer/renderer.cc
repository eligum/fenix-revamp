#include "fenix/renderer/renderer.hh"

namespace fenix {

    Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(const EditorCamera& camera)
    {
        s_SceneData->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_projection_view", s_SceneData->ProjectionViewMatrix);
        shader->SetMat4("u_transform", transform);

        vertex_array->Bind();
        RenderCommand::DrawIndexed(vertex_array);
        vertex_array->Unbind();
    }

    void Renderer::Submit(const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        auto shader = mesh->GetMaterial().GetShader();
        shader->Bind();
        shader->SetMat4("u_projection_view", s_SceneData->ProjectionViewMatrix);
        shader->SetMat4("u_transform", transform);
    }

} // namespace fenix
