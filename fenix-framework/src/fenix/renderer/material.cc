#include "fenix/renderer/material.hh"
#include <cstring>

namespace fenix {

    Material::Material(const Shader& shader, const std::string& name)
        : m_Name(name), m_Shader(shader)
    {
    }

    template <typename T>
    bool Material::Get(const std::string& key, T& value_) const
    {
        for (const auto& prop : m_Properties)
        {
            if (prop.Key == key)
            {
                if (prop.DataSize != sizeof(T))
                    continue;

                std::memcpy(&value_, prop.Data, sizeof(T));
                return true;
            }
        }

        return false;
    }

    template <typename T>
    bool Material::GetPropertyValue(const std::string& key, T& value_) const
    {
        auto iter = m_MatProperties.find(key);

        if (iter == m_MatProperties.end())
            return false;

        const auto& property = iter->second;
        if (std::holds_alternative<T>(property))
        {
            value_ = std::get<T>(property);
            return true;
        }

        return false;
    }

    auto Material::GetProperty(const std::string& key) const -> std::optional<MatProperty>
    {
        auto iter = m_MatProperties.find(key);

        return (iter != m_MatProperties.end()) ?
            std::optional<MatProperty>{iter->second} :
            std::nullopt;
    }

}
