add_executable(CISC220Final main.cpp
        ui.cpp
        murphy_util.cpp
        ../include/murphy_util.hpp)
target_include_directories(CISC220Final PRIVATE ../include)
target_link_libraries(CISC220Final PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

add_custom_command(
        TARGET CISC220Final POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:CISC220Final>/assets
)