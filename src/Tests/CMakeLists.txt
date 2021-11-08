cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_UNIT_TEST VERSION 0.1.0)

include(CTest)
include(GoogleTest)
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main Threads::Threads ${SERVER_LIBS})
gtest_discover_tests(${PROJECT_NAME})


