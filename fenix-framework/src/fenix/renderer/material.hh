#pragma once

#include <string>
#include <vector>
#include "fenix/renderer/shader.hh"

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
    inline constexpr auto MATKEY_BASE_COLOR = "mat.reflectivity";

    ///////////////////////////////////////////////////////////////////////////
    // Material data structures ///////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    enum class MaterialPropertyType
    {
        Float = 0x1,
        String = 0x2,
        Integer = 0x3,
        Buffer = 0x4
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

    class Material
    {
    public:
        Material(const Shader& shader);
        ~Material();

        /// Returns the name of the material.
        std::string GetName() const { return m_Name; }

        /// Removes all properties from the material.
        /// The data array remains allocated so adding new properties is quite fast.
        void Clear() { m_Properties.clear(); }

    private:
        std::string m_Name;
        std::vector<MaterialProperty> m_Properties;
        Shader m_Shader;
    };

} // namespace fenix
