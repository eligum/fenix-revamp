#include <glad/glad.h>
#include "fenix/renderer/mesh.hh"

namespace fenix {

    Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices, const Ref<Material>& material)
        : m_Vertices(std::move(vertices)),
          m_Indices(std::move(indices)),
          m_VAO(0),
          m_VBO(0),
          m_IBO(0),
          m_Material(material)
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

        // Position attribute
        glEnableVertexArrayAttrib(m_VAO, 0);
        glVertexArrayAttribBinding(m_VAO, 0, 0);
        glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, false, 0);

        // Normal attribute
        glEnableVertexArrayAttrib(m_VAO, 1);
        glVertexArrayAttribBinding(m_VAO, 1, 0);
        glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, false, 3 * sizeof(vertex_type));

        // Texture coordinates attribute
        glEnableVertexArrayAttrib(m_VAO, 2);
        glVertexArrayAttribBinding(m_VAO, 2, 0);
        glVertexArrayAttribFormat(m_VAO, 2, 3, GL_FLOAT, false, 6 * sizeof(vertex_type));

        glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 9 * sizeof(vertex_type));
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

} // namespace fenix
