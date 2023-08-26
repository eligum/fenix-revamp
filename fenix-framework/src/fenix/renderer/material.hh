#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include "fenix/renderer/shader.hh"

// TODO: The Material class still needs a lot of work but for now it just has
// the necessary elements to be minimally functional.

namespace fenix {

    ///////////////////////////////////////////////////////////////////////////
    // Material standard keys /////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    inline constexpr auto MATKEY_SHININESS = "mat.shininess";
    inline constexpr auto MATKEY_REFLECTIVITY = "mat.reflectivity";
    inline constexpr auto MATKEY_COLOR_AMBIENT = "clr.ambient";
    inline constexpr auto MATKEY_COLOR_DIFFUSE = "clr.diffuse";
    inline constexpr auto MATKEY_COLOR_SPECULAR = "clr.specular";
    inline constexpr auto MATKEY_COLOR_EMISSIVE = "clr.emissive";

    // ------------------------------------------------------------------------
    // PBR material properties
    // ------------------------------------------------------------------------
    inline constexpr auto MATKEY_BASE_COLOR = "mat.base_color";

    ///////////////////////////////////////////////////////////////////////////
    // Material data structures ///////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    enum class MaterialPropertyType : u32
    {
        Float = 0x1,
        Double = 0x2,
        Integer = 0x3,
        String = 0x4,
        Buffer = 0x5
    };

    struct MaterialProperty
    {
        /// Specifies the name of the property (key).
        std::string Key;

        /// Size in bytes of the buffer `Data` is pointing to.
        u32 DataSize;

        /// Type of the property.
        /// It defines the data layout inside the data buffer.
        MaterialPropertyType Type;

        /// Pointer to the binary buffer that holds the property's value.
        /// The size of the buffer is always `DataSize`.
        u8* Data;
    };

    /// Material class. Description pending.
    class Material
    {
    public:
        using MatProperty   = std::variant<i32, f32, f64, std::string>;
        using MatPropsTable = std::unordered_map<
            std::string,
            MatProperty
        >;

    public:
        Material(const Shader& shader, const std::string& name = "Default Name");
        ~Material() = default;

        /// Returns the name of the material.
        std::string GetName() const { return m_Name; }

        /// Retrieve a `T` value with a specific key from the material.
        /// @param key Key to search for. One of the MATKEY_XXX constants.
        /// @param value_ Reference that will receive the output value.
        /// @returns True on success false otherwise.
        template <typename T>
        bool Get(const std::string& key, T& value_) const;

        /// Retrieve a `T` value with a specific key from the material.
        /// @param key Key to search for. One of the MATKEY_XXX constants.
        /// @param value_ Reference that will receive the output value.
        /// @returns True on success false otherwise.
        template <typename T>
        bool GetPropertyValue(const std::string& key, T& value_) const;

        auto GetProperty(const std::string& key) const -> std::optional<MatProperty>;

        // template <typename T>
        // void AddProperty(const T& value, const std::string& key);

        /// Removes all properties from the material.
        /// The data array remains allocated so adding new properties is quite fast.
        void Clear() { m_Properties.clear(); }

    private:
        std::string m_Name;
        std::vector<MaterialProperty> m_Properties;
        MatPropsTable m_MatProperties;
        Shader m_Shader;
    };

} // namespace fenix
