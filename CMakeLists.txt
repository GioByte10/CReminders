cmake_minimum_required(VERSION 3.23)
project(CReminders)

set(CMAKE_CXX_STANDARD 20)

add_executable(CReminders main.cpp)
set_target_properties(CReminders PROPERTIES LINK_FLAGS "-static")

add_executable(commands commands.cpp)