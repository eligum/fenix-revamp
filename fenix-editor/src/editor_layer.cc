#include "editor_layer.hh"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <fenix/resource/resource_manager.hh>
#include <fenix/renderer/texture_2d.hh>

using namespace fenix;

void EditorLayer::OnAttach()
{
    Ref<Shader> shader = Shader::CreateFromFiles(
        "assets/shaders/test.vert",
        "assets/shaders/test.frag"
    );

    m_Material = CreateRef<Material>(std::move(shader));

    std::vector<Vertex> vertices = {
        // position             // color             // tex coords
        {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f},  {1.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f}},
    };
    std::vector<u32> indices = {
        0, 1, 2,
        2, 3, 0,
    };

    // Cube mesh
    m_CubeMesh = CreateRef<Mesh>(std::move(vertices), std::move(indices));
    m_CubeMesh->UploadToGPU();
    m_CubeMesh->ApplyMaterial(m_Material);

    // Watch Tower model
    auto& manager = ResourceManager::GetInstance();

    m_TowerModel = manager.Load<Model>("assets/models/watch-tower/obj/wooden watch tower2.obj");
    auto ok = manager.Load<Model>("assets/models/watch-tower/obj/wooden watch tower2.obj");
    const auto& model_meshes = m_TowerModel->GetMeshes();
    LOG_INFO("Tower model has {} meshes", model_meshes.size());
    for (const auto& mesh : model_meshes)
    {
        mesh->UploadToGPU();
        mesh->ApplyMaterial(m_Material);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_EditorCamera = CreateRef<EditorCamera>();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(fenix::TimeStep ts)
{
    RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    RenderCommand::Clear();

    m_EditorCamera->OnUpdate(ts);

    Renderer::BeginScene(*m_EditorCamera);

    Renderer::Submit(m_TowerModel);

    for (i32 i = 0; i < 4; ++i)
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, i * glm::radians(90.0f), Axis::Y);
        transform = glm::translate(transform, glm::vec3{0.0f, 0.0f, 0.5f});

        Renderer::Submit(m_CubeMesh, transform);
    }

    Renderer::EndScene();
}

void EditorLayer::OnEvent(fenix::Event& event)
{
    m_EditorCamera->OnEvent(event);

    // auto dispacher = EventDispatcher{event};
    // dispacher.Dispatch<KeyPressedEvent>(FENIX_BIND_EVENT_FN());
    // dispacher.Dispatch<MouseButtonPressedEvent>(FENIX_BIND_EVENT_FN());
}

void EditorLayer::OnRenderUI()
{
}
