cmake_minimum_required(VERSION 3.9.4)

project(sqlc)

add_executable(sqlc src/Token.cpp src/Module.cpp src/UIDRealm.t.h src/main.cpp)
target_compile_features(sqlc PRIVATE cxx_std_17)

add_executable(test src/UIDRealm.t.h src/test.cpp)
target_compile_features(test PRIVATE cxx_std_17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")