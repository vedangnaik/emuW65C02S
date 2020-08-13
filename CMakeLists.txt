cmake_minimum_required(VERSION 3.10)
project(emuW65C02S VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
    W65C02SLib STATIC 
    W65C02S.h
    W65C02S.cpp)

add_executable(emuW65C02S main.cpp)
target_link_libraries(emuW65C02S W65C02SLib)