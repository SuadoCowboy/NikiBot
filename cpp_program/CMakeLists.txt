cmake_minimum_required(VERSION 3.16)

project(NikiCMD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*)
add_executable(NikiCMD ${SOURCES})
target_include_directories(NikiCMD PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

target_link_directories(NikiCMD PRIVATE ${PROJECT_SOURCE_DIR}/libs)
target_link_libraries(NikiCMD NikiScript)