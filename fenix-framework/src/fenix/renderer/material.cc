#include "fenix/renderer/material.hh"

namespace fenix {

    Material::Material(const Shader& shader)
    {
        m_Shader = shader;
    }

}
