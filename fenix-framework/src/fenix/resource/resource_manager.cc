#include "fenix/resource/resource_manager.hh"

#include <typeinfo>
#include <cxxabi.h>
#include "fenix/renderer/model.hh"
#include "fenix/renderer/texture_2d.hh"

namespace fs = std::filesystem;

namespace fenix {

    ResourceManager::ResourceManager()
        : m_LoadedResourcesCount(0)
    {
    }

    static auto get_readable_name(const std::type_info& ti) -> std::string
    {
        int status;
        char* name = abi::__cxa_demangle(ti.name(), nullptr, nullptr, &status);
        auto result = std::string{name};
        std::free(name);
        return result;
    }

    template <typename T>
    Ref<T> ResourceManager::Load(const std::filesystem::path& path)
    {
        static_assert(
            std::is_base_of_v<Resource, T>,
            "'T' is not a Resource!"
        );

        auto maybe_messy_path = fs::path(path);
        auto canonical_path = fs::weakly_canonical(maybe_messy_path);

        auto cache_entry = m_ResourceCache.find(canonical_path.string());
        if (cache_entry != m_ResourceCache.end())
        {
            auto handle = m_Resources[cache_entry->second];
            if (!handle.expired())
            {
                auto owning_handle = std::dynamic_pointer_cast<T>(handle.lock());
                if (owning_handle != nullptr) {
                    return owning_handle;
                } else {
                    CORE_LOG_WARN("Trying to load '{}' as a '{}', which was already loaded as a '{}'.",
                        canonical_path.filename().string(),
                        get_readable_name(typeid(T)),
                        get_readable_name(typeid(*handle.lock()))
                    );
                    CORE_LOG_WARN("This is allowed but it is most likely a sign that there is a mistake in your code.");
                    CORE_LOG_WARN("For example, you could be trying to load an image as a 'Mesh' resource, which will cause problems later.");
                }
            }
            else
            {
                // Remove expired entry from cache
                m_ResourceCache.erase(cache_entry);
            }
        }

        auto ret_handle = CreateRef<T>(path);

        if (!ret_handle)
        {
            CORE_LOG_ERROR("Failed to load '{}' as a '{}'.",
                canonical_path.string(),
                get_readable_name(typeid(T))
            );
            return nullptr;
        }

        auto uuid_gen = UUIDGenerator{};
        auto new_id = uuid_gen.generate();

        ret_handle->m_Name = "Default Name";
        ret_handle->m_Path = canonical_path.string();
        ret_handle->m_UUID = new_id;

        m_Resources[new_id] = ret_handle;
        m_ResourceCache[canonical_path.string()] = new_id;
        ++m_LoadedResourcesCount;

        return ret_handle;
    }

    // Explicit template instantiation.
    //
    // Tells the compiler to generate code for specific template instances without having to
    // provide the full template definition. This can be useful for reducing compilation times
    // and controlling which template instances are included in your program.
    //
    // Since the header file only includes the declaration of the `Load()` function template
    // and its definition only exists in this source file, any other source file that tries to
    // use this function with a type that doesn't match one of the template instances listed
    // here, will cause a linking error due to an undefined reference, which is precisely the
    // desired behavior! Not all classes that inherit from `Resource` should be able to be loaded
    // from a single path.
    //
    // For once, C++ nuances play in our favor.

    template Ref<Model> ResourceManager::Load(const fs::path&);
    template Ref<Image> ResourceManager::Load(const fs::path&);
    template Ref<Texture2D> ResourceManager::Load(const fs::path&);

} // namespace fenix
