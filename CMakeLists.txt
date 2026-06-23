cmake_minimum_required(VERSION 3.28)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
project(rl_practice)
add_library(gridworld
    src/gridworld/gridworld.cpp

    src/base/buffer/nStepBuffer.cpp
    src/base/buffer/monteCarloBuffer.cpp

    src/base/policy/epsilonGreedyPolicy.cpp
    src/base/policy/greedyPolicy.cpp

    src/base/updater/QLearningUpdater.cpp
    src/base/updater/sarsaUpdater.cpp
    src/base/updater/monteCarloUpdater.cpp
    src/base/updater/expectedSarsaUpdater.cpp
    src/base/updater/doubleQLearningUpdater.cpp
    src/base/updater/nStepSarsaUpdater.cpp
    src/base/updater/nStepExpectedSarsaUpdater.cpp

    src/base/QValueEstimator/arithmeticMeanEstimator.cpp

    src/core/agent.cpp
    src/core/QValueSource.cpp
    src/core/policy.cpp
    src/base/updater/offPolicyNStepSarsaUpdater.cpp
    src/base/updater/nStepQLearningUpdater.cpp
)

add_executable(q_main q_main.cpp)
add_executable(s_main s_main.cpp)
add_executable(mc_main mc_main.cpp)

target_include_directories(gridworld PUBLIC
    include/backbone
    include/gridworld
)

target_link_libraries(q_main gridworld)
target_link_libraries(s_main gridworld)
target_link_libraries(mc_main gridworld)