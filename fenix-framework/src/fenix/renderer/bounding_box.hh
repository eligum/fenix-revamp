#pragma once

#include <iterator>
#include <type_traits>
#include <glm/vec3.hpp>
#include "fenix/utils/std_types.hh"
#include "fenix/core/base.hh"

namespace fenix {

    /// A 3D axis-aligned bounding box.
    class AABB
    {
    public:
        /// Default constructor.
        AABB();

        /// Creates an AABB from a range of floating point values,
        /// interpreted as 3D vertices, delimited by the `begin` and `end` iterators.
        template <typename Iter>
        AABB(Iter begin, Iter end);

        /// Creates bounding box object with minimum and maximum points initialized to
        /// `min_point` and `max_point` respectively.
        AABB(const glm::vec3& min_point, const glm::vec3& max_point)
            : m_MinPoint(min_point), m_MaxPoint(max_point) {}

        /// Returns the minimum point of the bounding box.
        const glm::vec3& GetMinPoint() const { return m_MinPoint; }

        /// Returns the maximum point of the bounding box.
        const glm::vec3& GetMaxPoint() const { return m_MaxPoint; }

        /// Returns the size of the bounding box along each axis.
        glm::vec3 GetSize() const { return m_MaxPoint - m_MinPoint; }

        /// Returns the center of the bounding box.
        glm::vec3 GetCenter() const { return (m_MaxPoint + m_MinPoint) / 2.0f; }

        /// Returns the center of the base of the bounding box.
        glm::vec3 GetBottomCenter() const
        {
            auto center = GetCenter();
            return { center.x, m_MinPoint.y, center.z };
        }

        /// Checks if the bounding box was created with the default constructor
        /// then never updated.
        bool IsValid() const;

        /// Returns true if the AABB contains `point`.
        bool ContainsPoint(const glm::vec3& point) const
        {
            return
                point.x >= m_MinPoint.x &&
                point.y >= m_MinPoint.y &&
                point.z >= m_MinPoint.z &&
                point.x <= m_MaxPoint.x &&
                point.y <= m_MaxPoint.y &&
                point.z <= m_MaxPoint.z;
        }

        /// Returns true if this AABB completely encloses `other`.
        bool Encloses(const AABB& other) const
        {
            return
                m_MinPoint.x <= other.m_MinPoint.x &&
                m_MinPoint.y <= other.m_MinPoint.y &&
                m_MinPoint.z <= other.m_MinPoint.z &&
                m_MaxPoint.x >= other.m_MaxPoint.x &&
                m_MaxPoint.y >= other.m_MaxPoint.y &&
                m_MaxPoint.z >= other.m_MaxPoint.z;
        }

        /// Updates the bounding box with a given `new_point`. Note that if the new
        /// point is already within the bounding box, this function does nothing.
        void Update(const glm::vec3& new_point);

        /// Update the bounding box with the points of another bounding box.
        void Update(const AABB& other)
        {
            Update(other.GetMinPoint());
            Update(other.GetMaxPoint());
        }

        /// Returns the smallest AABB that contains both this AABB and `other`.
        AABB Merge(const AABB& other) const
        {
            auto result = AABB(*this);
            result.Update(other);
            return result;
        }

        /// TODO: Implement this method.
        AABB ComputeIntersection(const AABB& other) const;

    private:
        glm::vec3 m_MinPoint;
        glm::vec3 m_MaxPoint;
    };

} // namespace fenix
