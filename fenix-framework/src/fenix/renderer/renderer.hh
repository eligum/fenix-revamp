#pragma once

#include "fenix/renderer/render_command.hh"
#include "fenix/renderer/shader.hh"
#include "fenix/renderer/camera.hh"

namespace fenix {

    class Renderer
    {
    public:
        static void BeginScene(const EditorCamera& camera);
        static void EndScene();

        // TODO: In the future this function should take a Mesh object. Also, the render
        // command should be put into a queue for later optimization. For instance, you
        // might want to render all object of the scene with the same material in one
        // draw call.
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array, const glm::mat4& trasnform = glm::mat4{1.0f});

    private:
        // TODO: Revise this design, the renderer should not have state.
        struct SceneData
        {
            glm::mat4 ProjectionViewMatrix;
        };

        static SceneData* s_SceneData;
    };

} // namespace fenix
