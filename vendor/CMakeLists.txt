set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLE OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(glfw)
add_subdirectory(glad-3.3)
add_subdirectory(glm)
add_subdirectory(spdlog)

# ---------------------------------------------------------------------------------------
# Group all dependencies
# ---------------------------------------------------------------------------------------

add_library(external_deps INTERFACE)
target_link_libraries(external_deps INTERFACE
                                    glfw
                                    glad
                                    glm
                                    spdlog)
