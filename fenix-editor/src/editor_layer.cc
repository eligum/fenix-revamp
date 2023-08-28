#include "editor_layer.hh"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

using namespace fenix;

void EditorLayer::OnAttach()
{
    Ref<Shader> shader = Shader::CreateFromFiles(
        "assets/shaders/test.vert",
        "assets/shaders/test.frag"
    );

    m_Material = CreateRef<Material>(std::move(shader));

    std::vector<f32> vertices = {
        // position          // color           // tex coords
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    };
    std::vector<Vertex> vertices2 = {
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

    // m_Mesh = CreateRef<Mesh>(std::move(vertices2), std::move(indices));
    // m_Mesh->UploadToGPU();
    // m_Mesh->SetMaterial(material);

    // indices = {
    //     0, 1, 2,
    //     2, 3, 0,
    // };

    auto vertex_buff = CreateRef<VertexBuffer>(reinterpret_cast<f32*>(vertices2.data()), vertices2.size() * sizeof(Vertex));
    auto index_buff = CreateRef<IndexBuffer>(indices.data(), indices.size());

    vertex_buff->SetLayout({
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float3, "a_color"   },
        {ShaderDataType::Float3, "a_texcoord"},
    });

    m_VertexArray = CreateRef<VertexArray>();
    m_VertexArray->SetVertexBuffer(vertex_buff);
    m_VertexArray->SetIndexBuffer(index_buff);

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

    /// End goal API
    // auto material = std::make_shared<Material>(m_Shader);
    // auto material_i = std::make_shared<MaterialInstance>(material); // For more specific data (degradation texture...)
    // material_i->SetValue("u_color", Color::Red);
    // material_i->SetTexture("u_albedo_map", texture) // How to create an albedo map with gimp?
    // mesh->Apply/SetMaterial(material_i);

    for (i32 i = 0; i < 4; ++i)
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, i * glm::radians(90.0f), Axis::Y);
        transform = glm::translate(transform, glm::vec3{0.0f, 0.0f, 0.5f});
        Renderer::Submit(m_Material->GetShader(), m_VertexArray, transform);

        // Renderer::Submit(m_Mesh, transform);
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
