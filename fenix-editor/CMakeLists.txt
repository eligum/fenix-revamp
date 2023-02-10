file(GLOB fenix_editor_SOURCES "src/**.cc")
file(GLOB fenix_editor_HEADERS "src/**.hh")

add_executable(fenix_editor ${fenix_editor_SOURCES} ${fenix_editor_HEADERS})

target_link_libraries(fenix_editor PRIVATE
                                   fenix-fw
                                   my_compile_warnings
                                   my_compile_features)

# A lil' bit of cmake fancyness
#add_custom_command(
#    TARGET fenix_editor
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fenix_editor>
#    "${CMAKE_CURRENT_LIST_DIR}")
