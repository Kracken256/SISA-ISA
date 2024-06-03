cmake_minimum_required(VERSION 3.0)
project(SISA C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "*.c")

foreach(SOURCE ${SOURCES})
    get_filename_component(NAME ${SOURCE} NAME_WE)
    add_executable(${NAME} ${SOURCE})
    target_compile_options(${NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -flto -O3)
    target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_link_libraries(${NAME} PRIVATE sisa_static)
    target_link_options(${NAME} PRIVATE -flto -O3 -s)
endforeach(SOURCE ${SOURCES})
