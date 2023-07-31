#include "editor_layer.hh"
#include <glad/glad.h>

using namespace fenix;

void EditorLayer::OnAttach()
{
    // Create shader from source files
    m_Shader.reset(Shader::FromGLSLTextFiles(
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
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(fenix::TimeStep ts)
{
    glClearColor(0.5f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Bind();

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    m_VertexArray->Unbind();
}

void EditorLayer::OnEvent(fenix::Event&)
{
}

void EditorLayer::OnRenderUI()
{
}
