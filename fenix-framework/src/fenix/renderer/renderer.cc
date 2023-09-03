#include "fenix/fxpch.hh"
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

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform)
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
        auto material = mesh->GetMaterial();
        auto shader = material->GetShader();

        shader->Bind();
        shader->SetMat4("u_projection_view", s_SceneData->ProjectionViewMatrix);
        shader->SetMat4("u_transform", transform);

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetIndexCount());
        mesh->Unbind();
    }

    void Renderer::Submit(const Ref<Model>& model, const glm::mat4& transform)
    {
        for (const auto& mesh : model->GetMeshes())
        {
            Renderer::Submit(mesh, transform);
        }
    }

} // namespace fenix
