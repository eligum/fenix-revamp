#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include "fenix/core/base.hh"
#include "fenix/core/uuid.hh"
#include "fenix/resource/resource.hh"
#include "fenix/utils/singleton.hh"

namespace fs = std::filesystem;

// TODO: Add the possibility to load resources using multiple threads.

namespace fenix {

    /// TODO.
    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        ~ResourceManager() = default;

        /// Load a Resource from disk.
        /// Once loaded from disk, further attempts to load a resource by `path` returns the same
        /// cached reference unless it has expired. A reference expires when the referenced resource
        /// is freed. Resources are automatically freed when they are no longer used.
        template <typename T>
        [[nodiscard]] Ref<T> Load(const std::filesystem::path& path);

        /// Desc.
        std::size_t GetLoadedResourceCount() const { return m_LoadedResourcesCount; }

        /// Desc.
        std::size_t GetResourceCount() const { return m_Resources.size(); }

        /// Desc.
        void Clear()
        {
            m_Resources.clear();
            m_ResourceCache.clear();
            m_LoadedResourcesCount = 0;
        }

    protected:
        ResourceManager();

    private:
        std::size_t m_LoadedResourcesCount;
        std::unordered_map<std::string, UUID> m_ResourceCache;
        std::unordered_map<UUID, std::weak_ptr<Resource>> m_Resources;
    };

} // namespace fenix
