cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)	
project(MAP_ECO VERSION 0.1.0)

#ENV CONFIGS 
set(LIB_TESTS ON)
set(BUILD_TYPE "SERVER_EXAMPLE")//VALUES: SERVER_EXAMPLE,LIB_SERVER.
#3rd 
include_directories(${PROJECT_SOURCE_DIR}/../3rd/asio-1.18.0/include)

#GTEST BEGIN ->MOVE TO A CONIFIG FILE
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

#Lib folders
add_subdirectory("./Serializer")
add_subdirectory("./Server")

list(APPEND SERVER_LIBS MAP_SERIALIZER)
list(APPEND SERVER_LIBS MAP_SERVER)

#CREAR EJECUTABLE PARA LAS PRUEBAS UNITARIAS
if (LIB_TESTS)
message("MAP SERVER TESTS ENABLED")
include(CTest)
enable_testing()
add_subdirectory("./Tests")
add_dependencies(MAP_UNIT_TEST MAP_SERIALIZER)
add_dependencies(MAP_UNIT_TEST MAP_SERVER)
endif()

#CREAR EJECUTABLE DEL SERVIDOR
#CREAR COMO LIBRERIA (OPCIONAL)
if (${BUILD_TYPE} STREQUAL "SERVER_EXAMPLE")
  add_executable(${PROJECT_NAME} ./Examples/cpp/server_example/server.cpp)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${SERVER_LIBS})
endif()



