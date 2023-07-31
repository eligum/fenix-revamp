#include "fenix/renderer/buffer.hh"
#include <glad/glad.h>

namespace fenix {

    ////////////////////////////////////////////////////////////////////////////
    // Vertex Buffer ///////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    VertexBuffer::VertexBuffer(const f32* vertices, u32 count) : m_Layout({})
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, count * sizeof(f32), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Index Buffer ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    IndexBuffer::IndexBuffer(const u32* indices, u32 count) : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, count * sizeof(u32), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ////////////////////////////////////////////////////////////////////////////
    // Vertex Array ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    static u32 ShaderDataType_to_GLSLType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Bool:   return GL_BOOL;
            default: break;
        }

        FENIX_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    void VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
    {
        FENIX_ASSERT(buffer->GetLayout().Count() > 0, "VertexBuffer can't have an empty layout!");

        const auto& layout = buffer->GetLayout();
        u32 index = 0; // attribute index

        for (const auto& element : layout)
        {
            glEnableVertexArrayAttrib(m_RendererID, index);
            glVertexArrayAttribBinding(m_RendererID, index, 0);
            glVertexArrayAttribFormat(m_RendererID,
                                      index,
                                      element.GetComponentCount(),
                                      ShaderDataType_to_GLSLType(element.type),
                                      element.normalize ? GL_TRUE : GL_FALSE,
                                      element.offset);
            ++index;
        }

        glVertexArrayVertexBuffer(m_RendererID, 0, buffer->GetID(), 0, layout.GetStride());
        m_VertexBuffer = buffer; // shared_ptr copy constructor
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
    {
        glVertexArrayElementBuffer(m_RendererID, buffer->GetID());
        m_IndexBuffer = buffer; // shared_ptr copy constructor
    }

    u32 VertexArray::GetIndexCount() const
    {
        return (m_IndexBuffer) ? m_IndexBuffer->GetCount() : 0;
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

} // namespace fenix
