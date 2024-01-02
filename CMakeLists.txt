cmake_minimum_required(VERSION 3.23)
project(design2_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")
include_directories(src)

#add_definitions(-DSIMULATION)

aux_source_directory(./src/ SRC_FILES)
add_executable(design2_project ${SRC_FILES})
