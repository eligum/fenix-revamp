option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLE OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

option(ASSIMP_BUILD_DOCS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INSTALL OFF)
option(ASSIMP_INSTALL_PDB OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(glfw)
add_subdirectory(glad-4.5)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(spdlog)
add_subdirectory(assimp)

# ---------------------------------------------------------------------------------------
# Group all dependencies
# ---------------------------------------------------------------------------------------

add_library(external_deps INTERFACE)
target_link_libraries(external_deps INTERFACE
                                    glfw
                                    glad
                                    glm
                                    stb
                                    spdlog
                                    assimp)
