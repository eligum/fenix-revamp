#include "fenix/renderer/shader.hh"
#include "fenix/core/base.hh"

#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace fenix {

    Shader* Shader::FromGLSLTextFiles(const std::string& vert_shader_path, const std::string& frag_shader_path)
    {
        Shader* shader = new Shader();
        shader->create_program_from_files(vert_shader_path, frag_shader_path);
        return shader;
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    // Marking a non-member function as static makes it only visible in this translation unit
    static std::string read_to_string(const std::string& filepath)
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
            CORE_LOG_ERROR("Failed to read file '{}'", filepath.c_str());
        }

        return result;
    }

    u32 Shader::compile_shader(GLenum type, const std::string& source)
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

    void Shader::create_program_from_files(const std::string& vert_shader_path, const std::string& frag_shader_path)
    {
        std::string vert_source = read_to_string(vert_shader_path);
        std::string frag_source = read_to_string(frag_shader_path);

        u32 programID = glCreateProgram();

        u32 vertShaderID = compile_shader(GL_VERTEX_SHADER, vert_source);
        glAttachShader(programID, vertShaderID);
        u32 fragShaderID = compile_shader(GL_FRAGMENT_SHADER, frag_source);
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
