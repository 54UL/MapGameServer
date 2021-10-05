cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP VERSION 0.1.0)

message("lol")
message(${PROJECT_SOURCE_DIR})

include_directories(
    ../../3rd/asio-1.18.0/include
    ${SERIALIZER_SRC})


file(GLOB_RECURSE  MGS_SOURCES
        src/*.hpp
        src/*.cpp)

find_package(Threads REQUIRED)
add_library(MAP STATIC ${MGS_SOURCES})
target_link_libraries(MAP Threads::Threads MAP_SERIALIZER)
set_target_properties(MAP PROPERTIES LINKER_LANGUAGE CXX)
target_sources(${PROJECT_NAME} PUBLIC ${MGS_SOURCES})