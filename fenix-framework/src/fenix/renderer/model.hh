#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include "fenix/renderer/mesh.hh"
#include "fenix/resource/resource_manager.hh"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace fenix {

    class Model : public Resource
    {
    public:
        Model(const std::filesystem::path& file_path) { load_model(file_path); }

        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    private:
        void load_model(const std::filesystem::path& file_path);
        void process_node(const aiNode* node, const aiScene* scene);
        static Mesh* copy_mesh(const aiMesh* mesh, const aiScene* scene);

    private:
        std::vector<Ref<Mesh>> m_Meshes;
    };

} // namespace fenix
