cmake_minimum_required(VERSION 3.16)
project(Final_Proyect_Programming_II)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(Final_Proyect_Programming_II
        main.cpp
        src/controller.cpp
        src/controller.h
        src/game_engine.cpp
        src/game_engine.h
        src/map.cpp
        src/map.h
        src/ux.cpp
        src/ux.h
)

