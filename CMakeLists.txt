cmake_minimum_required(VERSION 3.12)
project(Threads)

set(CMAKE_CXX_STANDARD 17)
file(GLOB lib_source_files "lib/*.cpp")

set (GTEST_LIB_PATH "/home/anil/lib/googletest-release-1.8.0/googletest/libgtest.a")
set (GTEST_INCLUDE_PATH "/home/anil/lib/googletest-release-1.8.0/googletest/include/")
file(GLOB TestSource "test/*.cpp")

include_directories(${GTEST_INCLUDE_PATH})
add_executable(Threads main.cpp ${lib_source_files} ${TestSource})
target_link_libraries(Threads ${GTEST_LIB_PATH} pthread)