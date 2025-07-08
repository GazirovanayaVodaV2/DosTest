cmake_minimum_required(VERSION 3.10)
project(dos-app)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu11 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS "src/*.c" "src/*.cpp" "src/*.h" "src/**/*.c" "src/**/*.cpp" "src/**/*.h" )

add_executable(main ${sources})

set(RAW_ASSETS "${CMAKE_SOURCE_DIR}/raw")
set(PROCESSED_ASSETS "${CMAKE_SOURCE_DIR}/assets")

add_custom_target(
    compile_assets ${CMAKE_COMMAND} 
)


add_subdirectory(src/graphics)
add_subdirectory(src/fps_timer)
add_subdirectory(src/array)
add_subdirectory(src/vector)
add_subdirectory(src/game_logic)

add_custom_target(assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
target_link_libraries(main m graphics fps array vector game)
add_dependencies(main assets compile_assets)