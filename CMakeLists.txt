cmake_minimum_required(VERSION 3.0)
project(SISA C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(sisa SHARED ${SOURCES})
target_include_directories(sisa PUBLIC "include")
target_compile_options(sisa PRIVATE -Wall -Wextra -Werror -pedantic -fvisibility=hidden -O3 -flto -march=native -mtune=native)
target_link_options(sisa PRIVATE -flto -march=native -mtune=native -Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/exports.map -s)

add_library(sisa_static STATIC ${SOURCES})
target_include_directories(sisa_static PUBLIC "include")
target_compile_options(sisa_static PRIVATE -Wall -Wextra -Werror -pedantic -fvisibility=hidden -O3 -march=native -mtune=native)
target_link_options(sisa_static PRIVATE -march=native -mtune=native -Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/exports.map -s)

add_subdirectory(samples)