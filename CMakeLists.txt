cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(YuvPlayer)

set(PLAYER_NAME "YUVPlayer")

aux_source_directory("." PLAYER_SRC)

add_executable(${PLAYER_NAME} ${PLAYER_SRC})

target_include_directories(${PLAYER_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 sdl2 REQUIRED IMPORTED_TARGET)

target_link_libraries(${PLAYER_NAME} PRIVATE PkgConfig::SDL2)

add_subdirectory(base)

add_subdirectory(media_base)

target_link_libraries(${PLAYER_NAME}
    PRIVATE
    base
    mediabase
)