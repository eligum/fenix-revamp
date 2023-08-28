#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include "fenix/renderer/mesh.hh"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace fenix {

    class Model
    {
    public:
        Model(const std::filesystem::path& file_path) { LoadModel(file_path); }

        void LoadModel(const std::filesystem::path& file_path);
        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    private:
        void process_node(const aiNode* node, const aiScene* scene);
        static Mesh* copy_mesh(const aiMesh* mesh, const aiScene* scene);

    private:
        std::vector<Ref<Mesh>> m_Meshes;
        std::filesystem::path m_Path;
    };

} // namespace fenix
