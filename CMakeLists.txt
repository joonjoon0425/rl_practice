cmake_minimum_required(VERSION 3.28)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(rl_practice)
add_library(gridworld
    src/gridworld/gridworld.cpp
    src/gridworld/sarsa.cpp
)

add_executable(main main.cpp)

target_include_directories(gridworld PUBLIC
    include/backbone
    include/gridworld
)

target_link_libraries(main gridworld)