cmake_minimum_required(VERSION 3.5)

project(neuroshima LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(neuroshima main.cpp gamecontroller.cpp army.cpp ArmyFactory.cpp)
