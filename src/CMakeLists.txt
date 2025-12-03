add_executable(CISC220Final main.cpp
        Player.cpp
        ../include/Player.hpp
        Structure.cpp
        ../include/Structure.hpp
        Cell.cpp
        ../include/Cell.hpp
        ui.cpp
        ../include/ui.hpp
        util.cpp
        ../include/util.hpp
        Board.cpp
        ../include/Board.hpp
        ../include/AppState.hpp
        game_logic.cpp
        game_logic.hpp
)
target_include_directories(CISC220Final PRIVATE ../include
        ../external/boost_1_89_0
)
target_link_libraries(CISC220Final PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)


add_custom_command(
        TARGET CISC220Final POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:CISC220Final>/assets
)