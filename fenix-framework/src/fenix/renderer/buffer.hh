#pragma once

#include <vector>
#include <string>
#include "fenix/core/base.hh"
#include "fenix/utils/std_types.hh"

// clang-format off
namespace fenix {

    enum class ShaderDataType
    {
        None = 0,
        Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Bool,
    };

    static u32 ShaderDataType_size(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 4 * 2;
            case ShaderDataType::Int3:   return 4 * 3;
            case ShaderDataType::Int4:   return 4 * 4;
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3:   return 4 * 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4 * 4;
            case ShaderDataType::Bool:   return 1;
            default: break;
        }

        FENIX_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    /// This class is a clean and elegant solution to the incredible error prone process of
    /// sending vertices to the OpenGL graphics pipeline. A `BufferLayout` represents how the
    /// different vertex attributes are arranged inside the array of data that we are sending
    /// to the vertex shader.
    class BufferLayout
    {
    public:
        /// A `BufferLayout::Element` contains all the necessary information we will need to
        /// be able to specify to OpenGL the format of our vertex data.
        struct Element
        {
            ShaderDataType type;
            std::string name;
            u32 size;
            u32 offset;
            bool normalize;

            Element(ShaderDataType _type, const std::string& _name, bool _normalize = false)
                : type(_type),
                  name(_name),
                  size(ShaderDataType_size(_type)),
                  offset(0),
                  normalize(_normalize)
            {}

            /// Returns the number of components of the `ShaderDataType` associated with this
            /// `Element`. For instance, `ShaderDataType::Float3` would return 3.
            u32 GetComponentCount() const
            {
                switch (type)
                {
                    case ShaderDataType::Int:    return 1;
                    case ShaderDataType::Int2:   return 2;
                    case ShaderDataType::Int3:   return 3;
                    case ShaderDataType::Int4:   return 4;
                    case ShaderDataType::Float:  return 1;
                    case ShaderDataType::Float2: return 2;
                    case ShaderDataType::Float3: return 3;
                    case ShaderDataType::Float4: return 4;
                    case ShaderDataType::Mat3:   return 3 * 3;
                    case ShaderDataType::Mat4:   return 4 * 4;
                    case ShaderDataType::Bool:   return 1;
                    default: break;
                }

                FENIX_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
            }
        };

        using const_iterator = std::vector<Element>::const_iterator;

    public:
        /// Constructs a `BufferLayout` from an initializer list of `<BufferLayout::Element>`.
        /// You have to specify the `type` and `name` for each element but the `normalize`
        /// field is optional and defaults to `false`.
        BufferLayout(std::initializer_list<Element> elements_list)
            : m_Elements(elements_list), m_Stride(0)
        {
            calculate_offset_and_stride();
        }

        /// Returns the stride (number of bytes between consecutive vertices of the same vertex
        /// attribute).
        u32 GetStride() const { return m_Stride; }

        /// Returns the number of elements (vertex attributes) of the layout.
        u32 Count() const { return m_Elements.size(); }

        const_iterator begin() const { return m_Elements.cbegin(); }
        const_iterator end() const { return m_Elements.cend(); }

    private:
        void calculate_offset_and_stride()
        {
            u32 offset = 0;
            m_Stride = 0;
            for (auto& attrib : m_Elements)
            {
                attrib.offset = offset;
                offset += attrib.size;
                m_Stride += attrib.size;
            }
        }

    private:
        std::vector<Element> m_Elements;
        u32 m_Stride;
    };

    /// Represents a Buffer Object that stores vertex attributes.
    class VertexBuffer
    {
    public:
        /// Constructs a buffer from an array of vertices.
        ///
        /// Parameter `count` refers to the number of elements/vertices that are being passed.
        /// Creating a buffer this way also binds it to the appropiate buffer target. Any
        /// subsequent buffer-related oprations will affect the state of the currently bound
        /// buffer.
        VertexBuffer(const f32* vertices, u32 count);
        ~VertexBuffer();

        /// Returns this buffer's current `BufferLayout`.
        const BufferLayout& GetLayout() const { return m_Layout; }

        /// Applies the specified layout to this buffer.
        void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

        /// Returns the OpenGL ID of this buffer.
        u32 GetID() const { return m_RendererID; }

        /// Binds this buffer as the current `ARRAY_BUFFER`.
        [[deprecated]] void Bind() const;

        /// Unbinds any buffer bound to the target `ARRAY_BUFFER`.
        [[deprecated]] void Unbind() const;

    private:
        u32 m_RendererID;
        BufferLayout m_Layout;
    };

    /// Represents a Buffer Object that stores vertex indices.
    class IndexBuffer
    {
    public:
        /// Constructs a buffer from an array of indices.
        ///
        /// Parameter `count` refers to the number of elements/indices that are being passed.
        /// Creating a buffer this way also binds it to the appropiate buffer target. Any
        /// subsequent buffer-related oprations will affect the state of the currently bound
        /// buffer.
        IndexBuffer(const u32* indices, u32 count);
        ~IndexBuffer();

        /// Returns the element count of the buffer (the number of indices).
        u32 GetCount() const { return m_Count; }

        /// Returns the OpenGL ID of this buffer.
        u32 GetID() const { return m_RendererID; }

        /// Binds this buffer as the current `ELEMENT_ARRAY_BUFFER`.
        [[deprecated]] void Bind() const;

        /// Unbinds any buffer bound to the target `ELEMENT_ARRAY_BUFFER`.
        [[deprecated]] void Unbind() const;

    private:
        u32 m_RendererID;
        u32 m_Count;
    };
    // clang-format on

    /// In OpenGL, a vertex array is an abstraction that wraps some state, a vertex buffer(s)
    /// and, optionaly, an index buffer in a single unit. Before drawing, we simply bind
    /// a vertex array and call the appropriate drawing function. Which function to call,
    /// depends on wether or not we specified an index buffer when configuring the vertex
    /// array.
    ///
    /// The `VertexArray` class only supports one `VertexBuffer` per `VertexArray` object.
    /// This means that all the vertex attributes we intend to pass to the vertex shader,
    /// must be packed in a single `VertexBuffer`. And, a non-empty layout must be specified
    /// for the `VertexBuffer`.
    class VertexArray
    {
    public:
        /// Creates a new `VertexArray`.
        /// In OpenGL, a vertex array is an abstraction that wraps some state, a vertex buffer
        /// and, optionaly, an index buffer in a single unit. See the class description for more
        /// information.
        VertexArray();
        ~VertexArray();

        /// Associates a `VertexBuffer` to this `VertexArray`.
        void SetVertexBuffer(const Ref<VertexBuffer>& buffer);

        /// Associates an `IndexBuffer` to this `VertexArray`.
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);

        /// Returns the number of indices of the associated `IndexBuffer`, or 0 if one hasn't
        /// been set yet.
        u32 GetIndexCount() const;

        /// Updates the state of the current OpenGL context to prepare for drawing with this
        /// `VertexArray` configuration.
        void Bind() const;

        /// Resets the drawing configuration of the current OpenGL context. If you wish to draw
        /// with this `VertexArray` configuration again you will have to call `Bind()` before
        /// drawing.
        void Unbind() const;

    private:
        u32 m_RendererID;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

} // namespace fenix
