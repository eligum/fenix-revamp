#pragma once

#include <filesystem>
#include "fenix/core/uuid.hh"

namespace fenix {

    class Resource
    {
    public:
        Resource()          = default;
        virtual ~Resource() = default;

        auto GetName() const -> const std::string& { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }
        auto GetPath() const -> std::filesystem::path { return m_Path; }
        void SetPath(std::filesystem::path&& path) { m_Path = std::move(path).string(); }

        /// Returns a magic number that uniquely identifies this resource.
        UUID GetID() const { return m_UUID; }

    private:
        std::string m_Name;
        std::string m_Path;
        UUID m_UUID;

    private:
        friend class ResourceManager;
    };

}
