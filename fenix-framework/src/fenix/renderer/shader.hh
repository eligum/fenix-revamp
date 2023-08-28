#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>

#include "fenix/core/base.hh"
#include "fenix/utils/std_types.hh"

namespace fenix {

    class Shader
    {
    public:
        using Path = std::filesystem::path;

    public:
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetInt         (const std::string& name, i32 value);
        void SetIntArray    (const std::string& name, i32* values, u32 count);
        void SetFloat       (const std::string& name, f32 value);
        void SetFloat2      (const std::string& name, const glm::vec2& value);
        void SetFloat3      (const std::string& name, const glm::vec3& value);
        void SetFloat3Array (const std::string& name, f32* values, u32 count);
        void SetFloat4      (const std::string& name, const glm::vec4& value);
        void SetMat3        (const std::string& name, const glm::mat3& value);
        void SetMat4        (const std::string& name, const glm::mat4& value);

        u32 GetID() const { return m_RendererID; }
        static Ref<Shader> CreateFromFiles(const Path& vert_shader_path, const Path& frag_shader_path);
        static Ref<Shader> CreateFromSource(const std::string& vert_shader_src, const std::string& frag_shader_src);

    private:
        i32 get_uniform_location(const std::string& name);
        void create_program_from_source(const std::string& vert_shader_src, const std::string& frag_shader_src);

    private:
        u32 m_RendererID;
        std::unordered_map<std::string, i32> m_UniformLocationCache;
    };

} // namespace fenix
