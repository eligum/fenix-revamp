#pragma once

#include <iterator>
#include <type_traits>
#include <glm/vec3.hpp>
#include "fenix/utils/std_types.hh"
#include "fenix/core/base.hh"

namespace fenix {

    /// Class for computing the bounding box of points in 3D space.
    class BoundingBox
    {
    public:
        /// Creates a bounding box object from a range of floating point values,
        /// interpreted as 3D vertices, delimited by the `begin` and `end` iterators.
        template <typename Iter>
        BoundingBox(Iter begin, Iter end);

        /// Creates bounding box object with minimum and maximum points initialized to
        /// `min_point` and `max_point` respectively.
        BoundingBox(const glm::vec3& min_point, const glm::vec3& max_point)
            : m_MinPoint(min_point), m_MaxPoint(max_point) {}

        /// Returns the minimum point of the bounding box.
        const glm::vec3& GetMinPoint() const { return m_MinPoint; }

        /// Returns the maximum point of the bounding box.
        const glm::vec3& GetMaxPoint() const { return m_MaxPoint; }

        /// Returns the size of the bounding box along each axis.
        glm::vec3 ComputeSize() const { return m_MaxPoint - m_MinPoint; }

        /// Returns the center of the bounding box.
        glm::vec3 ComputeCenter() const { return (m_MaxPoint + m_MinPoint) / 2.0f; }

        /// Returns the center of the base of the bounding box.
        glm::vec3 ComputeBottomCenter() const
        {
            auto center = ComputeCenter();
            return { center.x, m_MinPoint.y, center.z };
        }

        /// Updates the bounding box with a given `new_point`. Note that if the new
        /// point is already within the bounding box, this function does nothing.
        void Update(const glm::vec3& new_point);

        /// Update the bounding box with the points of another bounding box object.
        void Update(const BoundingBox& other)
        {
            Update(other.GetMinPoint());
            Update(other.GetMaxPoint());
        }

    private:
        glm::vec3 m_MinPoint;
        glm::vec3 m_MaxPoint;
    };

} // namespace fenix
