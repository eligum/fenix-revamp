#include "fenix/resource/resource_manager.hh"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace fenix {

    ResourceManager::ResourceManager()
        : m_LoadedResourceCount(0)
    {
    }

} // namespace fenix
