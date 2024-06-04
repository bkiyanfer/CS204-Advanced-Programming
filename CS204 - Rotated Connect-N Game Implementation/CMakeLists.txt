cmake_minimum_required(VERSION 3.27)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled2 main.cpp
        Board.h
        Player.h
        Board.cpp
        Player.cpp)
