cmake_minimum_required(VERSION 3.6)
project(ZetaRF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(src)
add_subdirectory(examples/SimpleRxTx)

include_directories(configs)
