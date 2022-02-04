cmake_minimum_required(VERSION 3.17)

project(phyzzle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(phyzzle src/Main.cpp
                       src/Application.cpp
                       src/Application.h
                       src/Graphics.cpp
                       src/Graphics.h
                       src/Physics/Constants.h
                       src/Physics/Particle.cpp
                       src/Physics/Particle.h
                       src/Physics/Vec2.cpp
                       src/Physics/Vec2.h
)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

target_link_libraries(phyzzle PRIVATE SDL2::SDL2
                                      SDL2::SDL2main
                                      SDL2::SDL2_gfx
                                      SDL2::SDL2_image
)

if(MSVC)
  target_compile_options(phyzzle PRIVATE /W4 /wd4201)
else()
  target_compile_options(phyzzle PRIVATE -Wall -Wextra -pedantic)
endif()