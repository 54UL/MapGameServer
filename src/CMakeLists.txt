cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERVER VERSION 0.1.0)

include_directories(${PROJECT_SOURCE_DIR}/../3rd/asio-1.18.0/include)
    

add_subdirectory(./Serializer)
add_subdirectory(./Server)

list(APPEND SERVER_LIBS MAP_SERIALIZER)
list(APPEND SERVER_LIBS MAP_CORE)

add_executable(MAP_SERVER ./Examples/cpp/server_example/server.cpp)
target_link_libraries(MAP_SERVER PUBLIC ${SERVER_LIBS})
