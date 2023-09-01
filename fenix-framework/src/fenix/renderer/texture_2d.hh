#pragma once

#include <filesystem>
#include <glad/glad.h>

#include "fenix/core/image.hh"
#include "fenix/utils/std_types.hh"

namespace fenix {

    class Texture2D : public Resource
    {
    public:
        /// Loads a texture from disk directly to the GPU.
        Texture2D(const std::filesystem::path& path);

        /// Loads a texture from memory to the GPU.
        Texture2D(const Image& image);

        /// Loads a texture from memory to the GPU.
        Texture2D(Image&& image);
        /// Allocates memory in the GPU to store a texture of the specified dimensions.

        Texture2D(u32 width, u32 height);

        /// Deletes this texture from the GPU.
        ~Texture2D();

        /// Binds the texture to the specified texture unit.
        void Bind(u32 slot = 0) const;

        /// Fills the memory region of the texture with the given data.
        /// @param data Pointer to the data buffer.
        /// @param size Number of bytes (8 bits) in the data buffer.
        void SetData(void* data, u32 size);

        /// Returns the texture width in pixels.
        u32 GetWidth() const { return m_Width; }

        /// Returns the texture height in pixels.
        u32 GetHeight() const { return m_Height; }

        /// Returns the texture identifier generated by OpenGL.
        u32 GetRendererID() const { return m_RendererID; }

        bool operator==(const Texture2D& other) const { return m_RendererID == other.m_RendererID; }

    private:
        u32 m_Width;
        u32 m_Height;
        u32 m_RendererID;
        GLenum m_InternalFormat;
        GLenum m_DataFormat;
    };

} // namespace fenix
