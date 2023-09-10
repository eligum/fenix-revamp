#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "fenix/renderer/buffer.hh"
#include "fenix/renderer/material.hh"

// TODO: Think about how to efficently abstract the `Mesh` class away from its OpenGL specific implementation.

namespace fenix {

    enum class PrimitiveType
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TIRANGLE_STRIP
    };

    /// Mesh class. Description pending.
    class Mesh
    {
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal;
            glm::vec3 tex_coords;

            bool operator==(const Vertex& other) const;
        };

        using vertex_type     = Vertex;
        using index_type      = u32;
        using vertex_iterator = std::vector<vertex_type>::const_iterator;
        using index_iterator  = std::vector<index_type>::const_iterator;


    public:
        /// Constructs a mesh object from a vector of vertices and a vector of indices.
        /// You can optionally pass a material or add it later using `ApplyMaterial()`.
        /// This constructor uses move semantics for both vectors, which means that the
        /// given vectors will be empty after this call because their elements will be
        /// moved.
        ///
        /// This approach is much more efficient than copying the elements but, if a copy
        /// is what you need, refer to the `Mesh` copy constructor.
        Mesh(std::vector<vertex_type>&& vertices, std::vector<index_type>&& indices, Ref<Material>&& material = nullptr);

        /// Copy constructor.
        Mesh(const Mesh& other);

        /// Default destructor.
        ~Mesh();

        /// Sets the material that will be used for rendering the mesh object.
        /// There can only be one active material per mesh. Materials are very configurable
        /// and extendable, refer to the `Material` class for more details.
        void ApplyMaterial(const Ref<Material>& material) { m_Material = material; }

        /// Returns a reference-counting pointer to the `Material` currently associated with
        /// the mesh object.
        auto GetMaterial() const -> Ref<Material> { return m_Material; }

        /// Uploads the mesh data to the VRAM so it can be accessed by the GPU.
        /// This step is required at least once for every mesh that you want to render.
        /// You don't have to call this method again as long as the mesh data remains in VRAM,
        /// doing so will trigger a warning and nothing will be sent to VRAM in debug builds.
        void UploadToGPU();

        /// Deletes the mesh data from the VRAM so it can no longer be accessed by the GPU.
        /// If you want to render the mesh again you will have to call `UploadToGPU()` first.
        void DeleteFromGPU();

        /// Tells the GPU to use the drawing configuration of this mesh for rendering.
        /// You do not have to call this method directly as the `fenix::Renderer` already
        /// does it automatically.
        void Bind() const;

        /// Opposite of `Bind()`, this is aslo automatically called by the `fenix::Renderer`
        /// when needed.
        void Unbind() const;

        /// Returns the ID of the underlying OpenGL vertex array object.
        u32 GetRendererID() const { return m_VAO; }

        /// Returns a pair of iterators (begin, end) that provide read-only access over the
        /// vertices of the mesh.
        auto GetVertices() const -> std::pair<vertex_iterator, vertex_iterator> { return std::make_pair(m_Vertices.cbegin(), m_Vertices.cend()); }

        /// Returns a pair of iterators (begin, end) that provide read-only access over the
        /// indices of the mesh.
        auto GetIndices() const -> std::pair<index_iterator, index_iterator> { return std::make_pair(m_Indices.cbegin(), m_Indices.cend()); }

        /// Returns the number of vertices of the mesh.
        auto GetVertexCount() const -> std::size_t { return m_Vertices.size(); }

        /// Returns the number of indices of the mesh.
        auto GetIndexCount() const -> std::size_t { return m_Indices.size(); }

        /// Returns a copy of the vertices of the mesh.
        auto CopyVertices() const -> std::vector<vertex_type> { return m_Vertices; }

        /// Returns a copy of the indices of the mesh.
        auto CopyIndices() const -> std::vector<index_type> { return m_Indices; }

    private:
        std::vector<vertex_type> m_Vertices;
        std::vector<index_type> m_Indices;

        u32 m_VAO; // Vertex Array Object
        u32 m_VBO; // Vertex Buffer Object
        u32 m_IBO; // Index Buffer Object

        Ref<Material> m_Material;
    };

    /// A single face in a mesh, referring to multiple vertices.
    ///
    /// If `NumIndices` is 3 we call the face a 'triangle', for values > 3
    /// it's called a 'polygon' (hey, that's just a definition!).
    struct Face
    {
        /// Number of indices defining this face.
        /// The maximum value for this member is 4.
        u32 NumIndices;

        /// Pointer to the index array. The size of the array is `NumIndices`.
        u32* Indices;

        /// Default constructor.
        Face() : NumIndices(0), Indices(nullptr) {}

        /// Copy constructor.
        Face(const Face& other) : NumIndices(0), Indices(nullptr) { *this = other; }

        /// Default destructor.
        ~Face() { delete[] Indices; }

        /// Assignment operator. Copies the elements of the index array.
        Face& operator=(const Face& other)
        {
            if (&other == this)
                return *this;

            if (Indices != nullptr)
                delete[] Indices;
            NumIndices = other.NumIndices;
            if (NumIndices > 0)
            {
                Indices = new u32[NumIndices];
                std::copy(other.Indices, other.Indices + NumIndices, Indices);
            }
            else
            {
                Indices = nullptr;
            }

            return *this;
        }

        /// Equality operator. Checks whether the index array of two faces is identical.
        bool operator==(const Face& other) const
        {
            if (Indices == other.Indices)
                return true;

            if (Indices != nullptr && NumIndices != other.NumIndices)
                return false;

            if (Indices == nullptr)
                return false;

            for (u32 i = 0; i < NumIndices; ++i)
                if (Indices[i] != other.Indices[i])
                    return false;

            return true;
        }

        /// Inequality operator. Checks whether the index array of two faces are NOT equal.
        bool operator!=(const Face& other) const
        {
            return !(*this == other);
        }
    };

} // namespace fenix
