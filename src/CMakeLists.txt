cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERVER_EXAMPLE VERSION 0.1.0)


add_subdirectory(./MAP_Serializer)
add_subdirectory(./MAP_Server)

include_directories(MAP_SERVER ${MAP_SERVER})
add_executable(MAP_SERVER_EXAMPLE ./examples/cpp/server.cpp)
target_link_libraries(MAP_SERVER_EXAMPLE MAP_SERVER)