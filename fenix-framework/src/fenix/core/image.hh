#pragma once

#include "fenix/resource/resource.hh"

namespace fenix {

    class Image : public Resource
    {
    public:
        Image();
        Image(const std::filesystem::path& path);
    };

} // namespace fenix
