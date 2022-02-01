cmake_minimum_required(VERSION 3.17)

project(phyzzle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(phyzzle src/Main.cpp
)

find_package(SDL2 REQUIRED)

target_link_libraries(phyzzle PRIVATE SDL2::SDL2
                                      SDL2::SDL2main
)

if(MSVC)
  target_compile_options(phyzzle PRIVATE /W4 /wd4201)
else()
  target_compile_options(phyzzle PRIVATE -Wall -Wextra -pedantic)
endif()