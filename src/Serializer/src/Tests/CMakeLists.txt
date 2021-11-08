cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERIALIZER_TESTS VERSION 0.1.0)

file(GLOB_RECURSE  SERIALIZER_TEST_SRC
        ./suites/*.hpp)
message(${SERIALIZER_TEST_SRC})
#when tests enable run 
add_executable(MAP_SERIALIZER_TESTS ${SERIALIZER_TEST_SRC})
set_target_properties(MAP_SERIALIZER_TESTS PROPERTIES LINKER_LANGUAGE CXX)
# enable_testing()
# include(GoogleTest)
# 
# message("SERIALIZER TESTS...")