#include "fenix/fxpch.hh"
#include <glad/glad.h>
#include "fenix/renderer/mesh.hh"
#include "fenix/renderer/buffer.hh"

namespace fenix {

    Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices, Ref<Material>&& material)
        : m_Vertices(std::move(vertices)),
          m_Indices(std::move(indices)),
          m_VAO(0),
          m_VBO(0),
          m_IBO(0),
          m_Material(std::move(material))
    {}

    Mesh::Mesh(const Mesh& other)
        : m_Vertices(other.m_Vertices),
          m_Indices(other.m_Indices),
          m_VAO(0),
          m_VBO(0),
          m_IBO(0),
          m_Material(other.m_Material)
    {
        if (other.m_VAO != 0)
            UploadToGPU();
    }

    Mesh::~Mesh()
    {
        if (m_VAO != 0)
        {
            glDeleteVertexArrays(1, &m_VAO);
            glDeleteBuffers(1, &m_VBO);
            glDeleteBuffers(1, &m_IBO);
        }
    }

    void Mesh::UploadToGPU()
    {
#ifdef FENIX_DEBUG
        if (m_VAO != 0)
        {
            CORE_LOG_WARN("This mesh has already been sent to the GPU");
            return;
        }
#endif
        glCreateVertexArrays(1, &m_VAO);
        glCreateBuffers(1, &m_VBO);
        glCreateBuffers(1, &m_IBO);

        glNamedBufferData(m_VBO, m_Vertices.size() * sizeof(vertex_type), m_Vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(m_IBO, m_Indices.size() * sizeof(index_type), m_Indices.data(), GL_STATIC_DRAW);

        BufferLayout buff_layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float3, "a_color"   },
            {ShaderDataType::Float3, "a_normal"  },
            {ShaderDataType::Float3, "a_texcoord"},
        };

        u32 index = 0;

        for (const auto& attrib : buff_layout)
        {
            glEnableVertexArrayAttrib(m_VAO, index);
            glVertexArrayAttribBinding(m_VAO, index, 0);
            glVertexArrayAttribFormat(m_VAO, index,
                                      attrib.GetComponentCount(),
                                      GL_FLOAT,
                                      attrib.normalize ? GL_TRUE : GL_FALSE,
                                      attrib.offset);
            ++index;
        }

        glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, buff_layout.GetStride());
        glVertexArrayElementBuffer(m_VAO, m_IBO);
    }

    void Mesh::DeleteFromGPU()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_IBO);
        m_VAO = 0;
    }

    void Mesh::Bind() const
    {
        glBindVertexArray(m_VAO);
    }

    void Mesh::Unbind() const
    {
        glBindVertexArray(0);
    }

    bool Mesh::Vertex::operator==(const Vertex& other) const
    {
        if (position != other.position)
            return false;

        if (color != other.color)
            return false;

        if (normal != other.normal)
            return false;

        if (tex_coords != other.tex_coords)
            return false;

        return true;
    }

} // namespace fenix
