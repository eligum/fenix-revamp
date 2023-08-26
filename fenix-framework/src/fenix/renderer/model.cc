#include "fenix/renderer/model.hh"

#include "fenix/core/log.hh"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace fenix {

    void Model::LoadModel(const std::filesystem::path& file_path)
    {
        auto importer = Assimp::Importer{};

        const aiScene* scene = importer.ReadFile(
            file_path.string(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FlipUVs
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        {
            CORE_LOG_ERROR("Failed to import file {}", file_path.string());
            CORE_LOG_ERROR("Detail: {}", importer.GetErrorString());
            return;
        }

        m_Path = file_path;
        process_node(scene->mRootNode, scene);
    }

    void Model::process_node(aiNode* node, const aiScene* scene)
    {
    }

    void Model::process_mesh(aiMesh* mesh, const aiScene* scene)
    {
    }

} // namespace fenix
