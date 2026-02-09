cmake_minimum_required(VERSION 3.16)
project(LRU_Cache_Portfolio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lru src/lru.cpp)
add_executable(ttl src/ttl.cpp)
add_executable(threadsafe src/threadsafe.cpp)

target_include_directories(lru PRIVATE include)
target_include_directories(ttl PRIVATE include)
target_include_directories(threadsafe PRIVATE include)
