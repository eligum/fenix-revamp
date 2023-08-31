#include "fenix/renderer/bounding_box.hh"

namespace fenix {

    AABB::AABB()
        : m_MinPoint({ std::numeric_limits<f32>::max(),
                       std::numeric_limits<f32>::max(),
                       std::numeric_limits<f32>::max() }),
          m_MaxPoint({ std::numeric_limits<f32>::lowest(),
                       std::numeric_limits<f32>::lowest(),
                       std::numeric_limits<f32>::lowest() })
    {}

    template <typename Iter>
    AABB::AABB(Iter begin, Iter end)
    {
        static_assert(
            std::is_same_v<typename std::iterator_traits<Iter>::value_type, f32>,
            "Iterator value_type must be a floating point scalar type"
        );

        FENIX_ASSERT(
            (end - begin) % 3 == 0,
            "The number of elements in the range specified by the iterators must be a multiple of 3."
        );

        m_MinPoint = { *begin, *(begin + 1), *(begin + 2) };
        m_MaxPoint = { *begin, *(begin + 1), *(begin + 2) };

        for (auto& it = begin; it != end; it += 3)
        {
            for (i32 i = 0; i < 3; ++i)
            {
                auto value = *(it + i);
                if (value < m_MinPoint[i])
                {
                    m_MinPoint[i] = value;
                }
                else if (value > m_MaxPoint[i])
                {
                    m_MaxPoint[i] = value;
                }
            }
        }
    }

    bool AABB::IsValid() const
    {
        return
            m_MinPoint.x != std::numeric_limits<f32>::max()
            && m_MinPoint.y != std::numeric_limits<f32>::max()
            && m_MinPoint.z != std::numeric_limits<f32>::max()
            && m_MaxPoint.x != std::numeric_limits<f32>::lowest()
            && m_MaxPoint.y != std::numeric_limits<f32>::lowest()
            && m_MaxPoint.z != std::numeric_limits<f32>::lowest();
    }

    void AABB::Update(const glm::vec3& new_point)
    {
        for (i32 i = 0; i < 3; ++i)
        {
            if (new_point[i] < m_MinPoint[i]) m_MinPoint[i] = new_point[i];
            if (new_point[i] > m_MaxPoint[i]) m_MaxPoint[i] = new_point[i];
        }
    }

} // namespace fenix
