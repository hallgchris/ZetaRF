cmake_minimum_required(VERSION 3.6)
project(ZetaRF)

add_subdirectory(src)
add_subdirectory(examples/SimpleRxTx)

include_directories(configs)
