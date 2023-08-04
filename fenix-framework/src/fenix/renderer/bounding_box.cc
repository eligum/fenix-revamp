#include "fenix/renderer/bounding_box.hh"

namespace fenix {

    template <typename Iter>
    BoundingBox::BoundingBox(Iter begin, Iter end)
    {
        static_assert(
            std::is_same_v<typename std::iterator_traits<Iter>::value_type, f32>,
            "Iterator value_type must be a floating point type."
        );

        FENIX_ASSERT(
            (end - begin) % 3 == 0,
            "The number of elements in the range specified by the iterators must be a multiple of 3."
        );

        if (end - begin < 6)
        {
            CORE_LOG_ERROR("At least 2 vertices are needed to construct a `BoundingBox`");
            return;
        }

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

    void BoundingBox::Update(const glm::vec3& new_point)
    {
        for (i32 i = 0; i < 3; ++i) {
            if (new_point[i] < m_MinPoint[i]) {
                m_MinPoint[i] = new_point[i];
            }
            if (new_point[i] > m_MaxPoint[i]) {
                m_MaxPoint[i] = new_point[i];
            }
        }
    }

} // namespace fenix
