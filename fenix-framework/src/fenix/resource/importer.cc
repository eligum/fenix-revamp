#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

namespace fs = std::filesystem;

void ImportAssetFromFile(const fs::path& path)
{
    auto importer = Assimp::Importer{};

    const aiScene* scene = importer.ReadFile(
        path.string(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );

    if (scene == nullptr)
    {
        // Report error
        return;
    }

    // Process the scene data
}
