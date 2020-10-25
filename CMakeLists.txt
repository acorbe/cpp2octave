cmake_minimum_required(VERSION 3.10)

# set the project name
project(cpp2octave_demo)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

add_subdirectory("cpp2oct")

# add the executable
add_executable(${PROJECT_NAME}  cpp2octave.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}/cpp2oct")

target_link_libraries(${PROJECT_NAME} "cpp2oct")
