#include "fenix/fxpch.hh"
#include "fenix/renderer/material.hh"

namespace fenix {

    Material::Material(const Ref<Shader>& shader, const std::string& name)
        : m_Name(name), m_RenderPriority(0), m_Shader(shader)
    {
    }

    Material::Material(Ref<Shader>&& shader, const std::string& name)
        : m_Name(name), m_RenderPriority(0), m_Shader(std::move(shader))
    {
    }

    Material::Material(const Material& other)
        : m_RenderPriority(other.m_RenderPriority),
          m_Shader(other.m_Shader),
          m_Properties(other.m_Properties)
    {
        m_Name = std::move(other.m_Name + " copy");
    }

    // Material& Material::operator=(const Material& other)
    // {
    //     if (this != &other)
    //     {

    //     }

    //     return *this;
    // }

    template <typename T>
    bool Material::GetPropertyValue(const std::string& key, T& value_) const
    {
        auto iter = m_Properties.find(key);

        if (iter == m_Properties.end())
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
        auto iter = m_Properties.find(key);

        return (iter != m_Properties.end()) ?
            std::optional<MatProperty>{iter->second} :
            std::nullopt;
    }

    void Material::SetProperty(const std::string& key, const MatProperty& value)
    {
        m_Properties[key] = value;
    }

    bool Material::AddProperty(const std::string& key, const MatProperty& value)
    {
        if (m_Properties.find(key) != m_Properties.end())
        {
            CORE_LOG_WARN("Material '{}' already has property '{}' defined.", m_Name, key);
            return false;
        }

        m_Properties[key] = value;
        return true;
    }

}
