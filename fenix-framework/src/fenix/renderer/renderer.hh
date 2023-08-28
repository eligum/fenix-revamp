#pragma once

#include "fenix/renderer/render_command.hh"
#include "fenix/renderer/shader.hh"
#include "fenix/renderer/camera.hh"
#include "fenix/renderer/mesh.hh"
#include "fenix/renderer/model.hh"

// TODO: Render commands should be put into a queue for later optimization. For instance, you
// might want to render all object of the scene with the same material in a single draw call.

namespace fenix {

    class Renderer
    {
    public:
        static void BeginScene(const EditorCamera& camera);
        static void EndScene();

        /// This function has been deprecated and should only be used for debugging purpouses.
        /// Use the overload that accepts a `Mesh` instead.
        [[deprecated("Use the overload that accepts a 'fenix::Mesh' instead.")]]
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& trasnform = glm::mat4{1.0f});

        /// Draws a mesh to the current active scene applying the given transform.
        static void Submit(const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4{1.0f});

        /// Draws a model (set of meshes) to the current active scene applying the given transform
        /// to each of the model's meshes.
        static void Submit(const Ref<Model>& model, const glm::mat4& transform = glm::mat4{1.0f});

    private:
        // TODO: Revisit this design, the renderer should not have state.
        struct SceneData
        {
            glm::mat4 ProjectionViewMatrix;
        };

        static SceneData* s_SceneData;
    };

} // namespace fenix
