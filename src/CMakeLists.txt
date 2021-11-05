cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_SERVER VERSION 0.1.0)

#3rd 
include_directories(${PROJECT_SOURCE_DIR}/../3rd/asio-1.18.0/include)


#g tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(./Serializer)
add_subdirectory(./Server)

list(APPEND SERVER_LIBS MAP_SERIALIZER)
list(APPEND SERVER_LIBS MAP_CORE)

add_executable(MAP_SERVER ./Examples/cpp/server_example/server.cpp)
target_link_libraries(MAP_SERVER PUBLIC ${SERVER_LIBS})

