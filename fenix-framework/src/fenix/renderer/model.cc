#include "fenix/renderer/model.hh"

#include "fenix/core/log.hh"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace fenix {

    void Model::load_model(const std::filesystem::path& file_path)
    {
        auto importer = Assimp::Importer{};

        const aiScene* scene = importer.ReadFile(
            file_path.string(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_GenNormals
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            CORE_LOG_ERROR("Failed to import file {}", file_path.string());
            CORE_LOG_ERROR("Detail: {}", importer.GetErrorString());
            return;
        }

        m_Meshes.clear();
        process_node(scene->mRootNode, scene);
    }

    void Model::process_node(const aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for (u32 i = 0; i < node->mNumMeshes; ++i)
        {
            u32 index = node->mMeshes[i];
            m_Meshes.push_back(
                Ref<Mesh>{ copy_mesh(scene->mMeshes[index], scene) }
            );
        }

        // Then do the same for each of its children
        for (u32 i = 0; i < node->mNumChildren; ++i)
        {
            process_node(node->mChildren[i], scene);
        }
    }

    Mesh* Model::copy_mesh(const aiMesh* mesh, const aiScene* scene)
    {
        // Copy vertex data
        std::vector<Vertex> vertices;
        vertices.reserve(mesh->mNumVertices);

        for (u32 i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex = {
                { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
                { mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z },
                { 0.0f,                 0.0f,                 0.0f },
            };

            u32 uv_channel = 0;
            if (mesh->HasTextureCoords(uv_channel))
            {
                vertex.TexCoords = {
                    mesh->mTextureCoords[uv_channel][i].x,
                    mesh->mTextureCoords[uv_channel][i].y,
                    mesh->mTextureCoords[uv_channel][i].z
                };
            }
            vertices.push_back(vertex);
        }

        // Copy index data
        std::vector<u32> indices;
        indices.reserve(mesh->mNumFaces * 3);

        for (u32 i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (u32 j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // TODO: Copy material data.
        // An aiScene is guranteed to contain at least ONE material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Create a fenix::Mesh on the heap and return pointer to it
        return new Mesh(std::move(vertices), std::move(indices));
    }

} // namespace fenix
