#include <string>
#include <filesystem>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
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
        static Shader* Create(const Path& vert_shader_path, const Path& frag_shader_path);

    private:
        u32 compile_shader(GLenum type, const std::string& source);
        void create_program_from_files(const Path& vert_shader_path, const Path& frag_shader_path);
        i32 get_uniform_location(const std::string& name);

    private:
        u32 m_RendererID;
        std::unordered_map<std::string, i32> m_UniformLocationCache;
    };

} // namespace fenix
