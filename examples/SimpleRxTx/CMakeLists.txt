cmake_minimum_required(VERSION 3.6)
project(simplerxtx)

include_directories(../../src)

add_executable(${PROJECT_NAME} SimpleRxTx.cpp)
target_link_libraries(${PROJECT_NAME} wiringPi)
target_link_libraries(${PROJECT_NAME} zetarf)
