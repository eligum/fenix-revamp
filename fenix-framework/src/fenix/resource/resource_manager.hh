#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <assimp/scene.h>
#include "fenix/core/base.hh"
#include "fenix/core/uuid.hh"
#include "fenix/utils/singleton.hh"

// TODO: Add the possibility to load resources using multiple threads.

namespace fenix {

    // class Resource;
    // class ResourceHandle;

    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        ~ResourceManager() = default;

        /// Desc.
        std::size_t GetLoadedResourceCount() const { return m_LoadedResourceCount; }

        /// Desc.
        std::size_t GetResourceCount() const { return m_Resources.size(); }

        /// Desc.
        void Clear() { m_Resources.clear(); }

        void Test()
        {
            CORE_LOG_INFO("Num. assets = {}", m_Resources.size());
            auto uuid_gen = UUIDGenerator();
            m_Resources[uuid_gen.generate()] = "Miguel";
        }

    protected:
        ResourceManager();

    private:
        std::size_t m_LoadedResourceCount;
        std::unordered_map<UUID, std::string> m_Resources;
    };

} // namespace fenix
