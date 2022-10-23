cmake_minimum_required(VERSION 3.21)
project(geometry_wars)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(geometry_wars main.cpp Vec2.cpp Vec2.h EntityManager.cpp EntityManager.h Entity.cpp Entity.h Game.cpp Game.h Common.h Components.h)
target_link_libraries(geometry_wars sfml-graphics)
