#pragma once

#include <fenix.hh>
#include <fenix/renderer/buffer.hh>
#include <fenix/renderer/shader.hh>
#include <fenix/renderer/camera.hh>

class EditorLayer : public fenix::Layer
{
public:
    EditorLayer() = default;
    ~EditorLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(fenix::TimeStep ts) override;
    void OnEvent(fenix::Event&) override;
    void OnRenderUI() override;

private:
    std::shared_ptr<fenix::Shader> m_Shader;
    std::shared_ptr<fenix::VertexArray> m_VertexArray;
    std::shared_ptr<fenix::EditorCamera> m_EditorCamera;
};
