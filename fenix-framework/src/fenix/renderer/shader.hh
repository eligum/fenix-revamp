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
        Shader() = default;
        Shader(const std::filesystem::path& vert_shader_path, const std::filesystem::path& frag_shader_path);
        Shader(const std::string& vert_shader_src, const std::string& frag_shader_src);
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

        auto GetUniformList(bool get_groups = false) const -> std::vector<std::string>;
        u32 GetRendererID() const { return m_RendererID; }

        static Ref<Shader> CreateFromFiles(const std::filesystem::path& vert_shader_path, const std::filesystem::path& frag_shader_path);
        static Ref<Shader> CreateFromSource(const std::string& vert_shader_src, const std::string& frag_shader_src);

    private:
        i32 get_uniform_location(const std::string& name);
        void create_program_from_source(const std::string& vert_shader_src, const std::string& frag_shader_src);

    private:
        u32 m_RendererID;
        std::unordered_map<std::string, i32> m_UniformLocationCache;
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader, std::string name);
        void Add(const Ref<Shader>& shader);
        bool Has(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };

} // namespace fenix
