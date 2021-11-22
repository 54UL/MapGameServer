cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERVER VERSION 0.1.0)

include_directories(
    ../Serializer/src)

file(GLOB_RECURSE  MGS_SOURCES
        src/*.hpp
        src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${MGS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
