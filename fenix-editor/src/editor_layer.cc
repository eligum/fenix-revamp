#include "editor_layer.hh"
#include <glad/glad.h>

using namespace fenix;

void EditorLayer::OnAttach()
{
    // Create shader from source files
    m_Shader.reset(Shader::Create(
        "assets-2/shaders/test.vert",
        "assets-2/shaders/test.frag"
    ));

    const std::vector<f32> vertices = {
        // position          // color           // tex coord
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };
    const std::vector<u32> indices = {
        0, 1, 2,
        2, 3, 0,
    };

    auto vertex_buff = std::make_shared<VertexBuffer>(vertices.data(), vertices.size());
    auto index_buff = std::make_shared<IndexBuffer>(indices.data(), indices.size());

    vertex_buff->SetLayout({
        {ShaderDataType::Float3, "a_position"},
        {ShaderDataType::Float3, "a_color"   },
        {ShaderDataType::Float2, "a_texcoord"},
    });

    m_VertexArray = std::make_shared<VertexArray>();
    m_VertexArray->SetVertexBuffer(vertex_buff);
    m_VertexArray->SetIndexBuffer(index_buff);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_EditorCamera = std::make_shared<EditorCamera>();
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

    Renderer::Submit(m_Shader, m_VertexArray);

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
