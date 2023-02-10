file(GLOB_RECURSE FENIX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
file(GLOB_RECURSE FENIX_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh")

if(FENIX_BUILD_SHARED)
    add_library(fenix-fw SHARED ${FENIX_SOURCES} ${FENIX_HEADERS})
else()
    add_library(fenix-fw STATIC ${FENIX_SOURCES} ${FENIX_HEADERS})
endif()

target_link_libraries(fenix-fw PUBLIC
                               external_deps
                               PRIVATE
                               my_compile_warnings
                               my_compile_features)

target_include_directories(fenix-fw PUBLIC include)
