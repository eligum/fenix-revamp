#include "fenix/renderer/texture_2d.hh"
#include "fenix/core/base.hh"

#include <glad/glad.h>
#include <stb_image.h>

namespace fs = std::filesystem;

namespace fenix {

    Texture2D::Texture2D(u32 width, u32 height)
        : m_Width(width), m_Height(height)
    {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        // Texture scaling and wrapping policy
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture2D::Texture2D(const fs::path& path)
        : m_Path(path)
    {
        i32 width;
        i32 height;
        i32 channels;
        stbi_set_flip_vertically_on_load(true);
        u8* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);

        FENIX_ASSERT(data, "Failed to load image!");

        m_Width = static_cast<u32>(width);
        m_Height = static_cast<u32>(height);

        GLenum internal_format = 0; // How OpenGL should store the texture data in the GPU
        GLenum data_format = 0;     // Format of the texture in memory

        if (channels == 4)
        {
            internal_format = GL_RGBA8;
            data_format = GL_RGBA;
        }
        else if (channels == 3)
        {
            internal_format = GL_RGB8;
            data_format = GL_RGB;
        }

        m_InternalFormat = internal_format;
        m_DataFormat = data_format;

        FENIX_ASSERT(internal_format | data_format, "Unsupported format!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internal_format, m_Width, m_Height);

        // Texture scaling and wrapping policy
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, data_format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture2D::SetData(void* data, [[maybe_unused]] u32 size)
    {
        [[maybe_unused]] u32 bpp = (m_DataFormat == GL_RGBA) ? 4 : 3; // bytes per pixel
        FENIX_ASSERT(size == m_Width * m_Height * bpp, "Data must fill the entire texture region!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture2D::Bind(u32 slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

} // namespace fenix
