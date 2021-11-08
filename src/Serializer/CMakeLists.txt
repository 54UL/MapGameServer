cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERIALIZER VERSION 1.0.0)


file(GLOB_RECURSE  SERIALIZER_SRC
        ./src/*.hpp
        ./src/*.cpp)
        
add_subdirectory(src/Tests)

add_library(${PROJECT_NAME} STATIC ${SERIALIZER_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

