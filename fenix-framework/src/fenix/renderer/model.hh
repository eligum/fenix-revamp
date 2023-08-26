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

    private:
        void process_node(aiNode* node, const aiScene* scene);
        void process_mesh(aiMesh* mesh, const aiScene* scene);

    private:
        std::vector<Mesh> m_Meshes;
        std::filesystem::path m_Path;
    };

} // namespace fenix
