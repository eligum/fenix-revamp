#include "fenix/fxpch.hh"
#include "fenix/renderer/shader.hh"
#include "fenix/core/base.hh"

#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

namespace fenix {

    static std::string read_to_string(const fs::path& filepath);
    static u32 compile_shader(GLenum type, const std::string& source);

    Ref<Shader> Shader::CreateFromFiles(const fs::path& vert_shader_path, const fs::path& frag_shader_path)
    {
        std::string vert_source = read_to_string(vert_shader_path);
        std::string frag_source = read_to_string(frag_shader_path);

        auto shader = CreateRef<Shader>();
        shader->create_program_from_source(vert_source, frag_source);
        shader->set_code(vert_source + frag_source);
        return shader;
    }

    Ref<Shader> Shader::CreateFromSource(const std::string& vert_shader_src, const std::string& frag_shader_src)
    {
        auto shader = CreateRef<Shader>();
        shader->create_program_from_source(vert_shader_src, frag_shader_src);
        shader->set_code(vert_shader_src + frag_shader_src);
        return shader;
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    // Marking a function as static makes it visible in this translation unit only
    static std::string read_to_string(const fs::path& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in);

        if (in) {
            in.seekg(0, std::ios::end);
            result.resize(static_cast<size_t>(in.tellg()));
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.length());
            in.close();
        } else {
            CORE_LOG_ERROR("Failed to read file '{}'", filepath.string());
        }

        return result;
    }

    // Marking a function as static makes it visible in this translation unit only
    static u32 compile_shader(GLenum type, const std::string& source)
    {
        u32 shaderID = glCreateShader(type);

        const char* shader_source = source.c_str();
        glShaderSource(shaderID, 1, &shader_source, nullptr);
        glCompileShader(shaderID);

        i32 success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success) {
            i32 maxLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> infoLog(maxLength);
            glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(shaderID);

            CORE_LOG_ERROR("{}", infoLog.data());
        }

        return shaderID;
    }

    void Shader::create_program_from_source(const std::string& vert_shader_src, const std::string& frag_shader_src)
    {
        u32 programID = glCreateProgram();

        // Vertex shader
        u32 vertShaderID = compile_shader(GL_VERTEX_SHADER, vert_shader_src);
        glAttachShader(programID, vertShaderID);

        // Fragment shader
        u32 fragShaderID = compile_shader(GL_FRAGMENT_SHADER, frag_shader_src);
        glAttachShader(programID, fragShaderID);

        glLinkProgram(programID);

        i32 success = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);

        if (!success) {
            i32 maxLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(programID);
            glDeleteShader(vertShaderID);
            glDeleteShader(fragShaderID);

            CORE_LOG_ERROR("{}", infoLog.data());
        }

        glDetachShader(programID, vertShaderID);
        glDetachShader(programID, fragShaderID);
        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);

        m_RendererID = programID;
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    auto Shader::GetUniformList() const -> std::vector<std::string>
    {
        auto uniforms = std::vector<std::string>{};
        auto source = std::string_view{m_Code};

        std::size_t index = 0;
        std::size_t N = source.length();
        while (index < N)
        {
            index = source.find("uniform", index);

            if (index == std::string::npos)
                break;

            // Skip if 'uniform' token is inside a comment
            std::size_t i = source.rfind("//", index) + 2;
            if (i != std::string::npos)
            {
                bool is_comment = true;
                while (i < index)
                {
                    if (source[i] == '\n')
                    {
                        is_comment = false;
                        break;
                    }
                    ++i;
                }
                if (is_comment)
                {
                    index = index + std::size("uniform") - 1;
                    continue;
                }
            }

            index = source.find(';', index);
            std::size_t start = source.rfind(' ', index) + 1;
            std::size_t count = index - start;
            uniforms.emplace_back(source.substr(start, count));
        }

        return uniforms;
    }

    i32 Shader::get_uniform_location(const std::string& name)
    {
        auto it = m_UniformLocationCache.find(name);

        if (it != m_UniformLocationCache.end()) {
            return it->second;
        } else {
            i32 location = glGetUniformLocation(m_RendererID, name.c_str());
            m_UniformLocationCache[name] = location;
            return location;
        }
    }

    void Shader::SetInt(const std::string& name, i32 value)
    {
        glUniform1i(get_uniform_location(name), value);
    }

    void Shader::SetIntArray(const std::string& name, i32* values, u32 count)
    {
        glUniform1iv(get_uniform_location(name), count, values);
    }

    void Shader::SetFloat(const std::string& name, f32 value)
    {
        glUniform1f(get_uniform_location(name), value);
    }

    void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        glUniform2f(get_uniform_location(name), value.x, value.y);
    }

    void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
    }

    void Shader::SetFloat3Array(const std::string& name, f32* values, u32 count)
    {
        glUniform3fv(get_uniform_location(name), count, values);
    }

    void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& matrix)
    {
        glUniformMatrix3fv(get_uniform_location(name), 1, false, glm::value_ptr(matrix));
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(get_uniform_location(name), 1, false, glm::value_ptr(matrix));
    }

} // namespace fenix
