cmake_minimum_required(VERSION 3.16)

project(NikiCMD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(NikiCMD ${PROJECT_SOURCE_DIR}/src/Main.cpp)
target_include_directories(NikiCMD PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_directories(NikiCMD PRIVATE ${PROJECT_SOURCE_DIR}/libs)
target_link_libraries(NikiCMD NikiScript)
