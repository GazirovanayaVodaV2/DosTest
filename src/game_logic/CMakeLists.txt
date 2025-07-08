cmake_minimum_required(VERSION 3.10)

# Название проекта и версия
project(game-logic VERSION 0.1.0)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu11 -save-temps)



file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE HEADERS "*.h")

add_library(game ${SOURCES} ${HEADERS})