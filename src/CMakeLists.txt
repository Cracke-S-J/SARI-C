cmake_minimum_required (VERSION 3.16)

project(sari-c)

set(CMAKE_BUILD_TYPE "Debug")

# set(CMAKE_C_FLAGS "-Wall -Werror")
# set(CMAKE_CXX_FLAGS "-Wall -Werror")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /tp ")

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(fe be test utils)
# add_subdirectory(test/testlex)
# add_subdirectory(test/testparser)
file(GLOB FE_CPP_FILE ./fe/*.cpp)
file(GLOB UTILS_CPP_FILE ./utils/*.cpp)
add_executable(Sari-C sari.cpp "${FE_CPP_FILE}" "${UTILS_CPP_FILE}")
