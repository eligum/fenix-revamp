#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "fenix/utils/std_types.hh"

namespace fenix {

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

    class Mesh
    {
    public:
        using Iter = std::vector<glm::vec3>::const_iterator;
    public:
        auto GetVertices() const -> const f32* { return &m_Vertices[0]; }
        auto GetVerticesIter() const -> std::pair<Iter, Iter> { std::make_pair(m_Vertices.cbegin(), m_Vertices.cend()); }
        auto GetVertexCount() const -> u32 { return static_cast<u32>(m_Vertices.size()); }

    private:
        std::vector<f32> m_Vertices;
        std::vector<glm::vec3> m_Normals;
        std::vector<u32> m_Indices;
        std::vector<Face> m_Faces;
    };

} // namespace fenix
