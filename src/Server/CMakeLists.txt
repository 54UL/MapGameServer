cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_CORE VERSION 0.1.0)

message("SERVER LIB CMAKE............")
message(${PROJECT_SOURCE_DIR})

include_directories(
    ../Serializer/src)

file(GLOB_RECURSE  MGS_SOURCES
        src/*.hpp
        src/*.cpp)

add_library(MAP_CORE STATIC ${MGS_SOURCES})
target_include_directories(MAP_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
