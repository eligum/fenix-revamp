#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include "fenix/core/base.hh"
#include "fenix/renderer/shader.hh"

// TODO: The Material class still needs a lot of work but for now it just has
// the necessary elements to be minimally functional.

namespace fenix {

    ///////////////////////////////////////////////////////////////////////////
    // Material properties standard keys //////////////////////////////////////
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
        /// Constructs a material object from a shader.
        Material(const Ref<Shader>& shader, const std::string& name = "Default Name");

        /// Constructs a material object from a shader.
        Material(Ref<Shader>&& shader, const std::string& name = "Default Name");

        /// Copy constructor.
        Material(const Material& other);

        /// Default destructor.
        virtual ~Material() = default;

        /// Copies and assigns values from another material object.
        Material& operator=(const Material& other) = default;

        /// Returns a reference-counting pointer to the `Shader` associated with
        /// the material.
        auto GetShader() const -> Ref<Shader> { return m_Shader; }

        auto GetRenderPriority() const -> u32 { return m_RenderPriority; }

        /// Returns the name of the material.
        auto GetName() const -> std::string { return m_Name; }

        /// Sets the material's name.
        void SetName(const std::string& name) { m_Name = name; }

        /// Sets the material's name using move semantics. Because why not, I just
        /// learnt about it and wanted to use it somewhere.
        void SetName(std::string&& name) { m_Name = std::move(name); }

        /// Retrieves the `T` value from the material property with the specified key.
        /// @param key Key to search for. One of the MATKEY_XXX constants.
        /// @param value_ Reference that will receive the output value.
        /// @returns True on success false otherwise.
        template <typename T>
        bool GetPropertyValue(const std::string& key, T& value_) const;

        /// Returns a material property with the specified key.
        /// The returned value is wrapped in std::optional because the material might
        /// not have a property with that key.
        auto GetProperty(const std::string& key) const -> std::optional<MatProperty>;

        /// Overwrites or adds a material property with the given value.
        void SetProperty(const std::string& key, const MatProperty& value);

        /// Adds a material property with the given value.
        /// Does nothing if the material has already defined a property with the specified key.
        bool AddProperty(const std::string& key, const MatProperty& value);

        /// Removes all properties from the material.
        /// The container where properties were stored remains allocated so adding
        /// new properties is quite fast.
        void Clear() { m_Properties.clear(); }

    private:
        std::string m_Name;
        u32 m_RenderPriority;
        Ref<Shader> m_Shader;
        MatPropsTable m_Properties;
    };

    class MaterialInstance : public Material
    {
    };

} // namespace fenix

// const char *aiTextureTypeToString(aiTextureType in) {
//     switch (in) {
//     case aiTextureType_NONE:
//         return "n/a";
//     case aiTextureType_DIFFUSE:
//         return "Diffuse";
//     case aiTextureType_SPECULAR:
//         return "Specular";
//     case aiTextureType_AMBIENT:
//         return "Ambient";
//     case aiTextureType_EMISSIVE:
//         return "Emissive";
//     case aiTextureType_OPACITY:
//         return "Opacity";
//     case aiTextureType_NORMALS:
//         return "Normals";
//     case aiTextureType_HEIGHT:
//         return "Height";
//     case aiTextureType_SHININESS:
//         return "Shininess";
//     case aiTextureType_DISPLACEMENT:
//         return "Displacement";
//     case aiTextureType_LIGHTMAP:
//         return "Lightmap";
//     case aiTextureType_REFLECTION:
//         return "Reflection";
//     case aiTextureType_BASE_COLOR:
//         return "BaseColor";
//     case aiTextureType_NORMAL_CAMERA:
//         return "NormalCamera";
//     case aiTextureType_EMISSION_COLOR:
//         return "EmissionColor";
//     case aiTextureType_METALNESS:
//         return "Metalness";
//     case aiTextureType_DIFFUSE_ROUGHNESS:
//         return "DiffuseRoughness";
//     case aiTextureType_AMBIENT_OCCLUSION:
//         return "AmbientOcclusion";
//     case aiTextureType_SHEEN:
//         return "Sheen";
//     case aiTextureType_CLEARCOAT:
//         return "Clearcoat";
//     case aiTextureType_TRANSMISSION:
//         return "Transmission";
//     case aiTextureType_UNKNOWN:
//         return "Unknown";
//     default:
//         break;
//     }
//     ai_assert(false);
//     return "BUG";
// }
