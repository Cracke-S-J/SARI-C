cmake_minimum_required (VERSION 3.16)

project(sari-c)

set(CMAKE_BUILD_TYPE "Debug")

# set(CMAKE_C_FLAGS "-Wall -Werror")
# set(CMAKE_CXX_FLAGS "-Wall -Werror")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG -DIs_True_On")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O2 -DNDEBUG")

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(fe be test utils)
add_subdirectory(test/testlex)
add_subdirectory(test/testparser)
# add_executable(Sari-C ${CMAKE_SOURCE_DIR}/sari.cpp)
