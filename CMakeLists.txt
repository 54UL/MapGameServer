cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP VERSION 0.1.0)


add_subdirectory(3rd/taskflow-master)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/3rd/asio-1.18.0/include
    ${PROJECT_SOURCE_DIR}/3rd/taskflow-master/taskflow
   )

file(GLOB MGS_SOURCES
        include/*.hpp
        src/*.cpp)

add_executable(${PROJECT_NAME})
set_target_properties(MAP PROPERTIES LINKER_LANGUAGE CXX)
target_sources(${PROJECT_NAME} PRIVATE ${MGS_SOURCES})
