# Checkout the project repo at https://github.com/eligum/gfx

cmake_minimum_required(VERSION 3.18)

# ----------------------------------------------------------------------------------------
# Start project
# ----------------------------------------------------------------------------------------
include(cmake/CompilerWarnings.cmake)

# Require that the compiler being used supports C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Fenix-Engine VERSION 1.0
                     DESCRIPTION "OpenGL 3D scene visualizer"
                     LANGUAGES CXX C)

# ----------------------------------------------------------------------------------------
# Project build configuration
# ----------------------------------------------------------------------------------------
include(cmake/BuildTypeSettings.cmake)

# Output all binaries under the same easy-to-access directory
if(NOT MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()

# Build configuration options
option(FENIX_BUILD_SANDBOX "Build the sandbox application (used for testing)" OFF)
option(FENIX_BUILD_SHARED "Build 'fenix-framework' as a shared library" OFF)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake
add_library(my_compile_warnings INTERFACE)
setup_warnings(my_compile_warnings)

# Link this "library" to binaries you want to build using c++17 standard
add_library(my_compile_features INTERFACE)
target_compile_features(my_compile_features INTERFACE cxx_std_17)

# ----------------------------------------------------------------------------------------
# Build project dependencies
# ----------------------------------------------------------------------------------------
add_subdirectory(vendor)

# ----------------------------------------------------------------------------------------
# Build project
# ----------------------------------------------------------------------------------------
add_subdirectory(fenix-framework)
add_subdirectory(fenix-editor)

if(FENIX_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
