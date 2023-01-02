cmake_minimum_required(VERSION 3.23)
project(CReminders)

set(CMAKE_CXX_STANDARD 20)
add_executable(CReminders main.cpp)
set_target_properties(CReminders PROPERTIES LINK_FLAGS "-static -mwindows")

add_executable(commands commands.cpp)
set_target_properties(commands PROPERTIES LINK_FLAGS "-static")

add_executable(test test.cpp)
set_target_properties(test PROPERTIES LINK_FLAGS "-static")

add_executable(messageBox messageBox.cpp)
set_target_properties(messageBox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ToastNotification/dist LINK_FLAGS "-static -mwindows")

