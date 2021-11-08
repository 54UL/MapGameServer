cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP VERSION 0.1.0)

#ENV CONFIGS 
set(LIB_TESTS ON)
set(BUILD_ENV "DEBUG")#BUILD_ENV = {DEBUG,PROD}
set(BUILD_TYPE "SERVER_EXAMPLE")#BUILD_TYPE = {LIB,SERVER_EXAMPLE}

#3rd 
include_directories(${PROJECT_SOURCE_DIR}/../3rd/asio-1.18.0/include)
find_package(Threads REQUIRED)

#GTEST BEGIN ->  TODO:MOVE TO A CONIFIG FILE
#g tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#GTEST END 

#MAP Lib folders
add_subdirectory(./Serializer)
add_subdirectory(./Server)

list(APPEND MAP_LIBS MAP_SERIALIZER)
list(APPEND MAP_LIBS MAP_SERVER)

#EXECUTABLE FOR UNIT TESTING
if (LIB_TESTS)
  message("MAP SERVER TESTS ENABLED")
  add_subdirectory(./Tests)
  add_dependencies(MAP_UNIT_TEST ${MAP_LIBS})
endif()

#BUILD DEFINITIONS
#LIB(default): does nothing because the proyect itself is already compiled as a lib
#SERVER_EXAMPLE: simple entry point instancing a basic map server 
#CLIENT_EXAMPLE: simple entry point instancing a basic map client

message("MAP BUILD TYPE:")
if (${BUILD_TYPE} STREQUAL "SERVER_EXAMPLE")
  message("EXECUTABLE SERVER EXAMPLE")
  add_executable(${PROJECT_NAME} ./Examples/cpp/server_example/server.cpp)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${MAP_LIBS})
endif()

