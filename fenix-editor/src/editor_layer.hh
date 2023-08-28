#pragma once

#include <fenix.hh>
#include <fenix/renderer/buffer.hh>
#include <fenix/renderer/shader.hh>
#include <fenix/renderer/mesh.hh>
#include <fenix/renderer/camera.hh>

namespace fenix {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer() = default;
        ~EditorLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(TimeStep ts) override;
        void OnEvent(Event&) override;
        void OnRenderUI() override;

    private:
        Ref<Shader> m_Shader;
        Ref<VertexArray> m_VertexArray;
        Ref<Mesh> m_Mesh;
        Ref<Material> m_Material;
        Ref<EditorCamera> m_EditorCamera;
    };

}
