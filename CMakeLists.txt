cmake_minimum_required(VERSION 3.10)
project(PIDHeaterSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pid_heater_sim
    src/main.cpp
    src/PID.cpp
    src/Zone.cpp
)
target_include_directories(pid_heater_sim PRIVATE src)
