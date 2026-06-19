cmake_minimum_required(VERSION 3.28)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
project(rl_practice)
add_library(gridworld
    src/gridworld/gridworld.cpp
    src/gridworld/qlearning.cpp
)

# add_executable(sarsa_main sarsa_main.cpp)
add_executable(q_main q_main.cpp)

target_include_directories(gridworld PUBLIC
    include/backbone
    include/gridworld
)

# target_link_libraries(sarsa_main gridworld)
target_link_libraries(q_main gridworld)