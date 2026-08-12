cmake_minimum_required(VERSION 3.20)
project(myapp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tensor STATIC 
    src/tensor/tensor.cpp
    src/tensor/tensor.h
)
target_include_directories(tensor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(bitvector STATIC 
    src/bitvector/bitvector.cpp
    src/bitvector/bitvector.h
)
target_include_directories(bitvector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(myapp app/main.cpp)
target_link_libraries(myapp PRIVATE tensor bitvector)