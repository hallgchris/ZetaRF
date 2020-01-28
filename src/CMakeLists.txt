cmake_minimum_required(VERSION 3.6)
project(zetarf)

include_directories(configs)
file(GLOB SOURCES "*.h" "*.cpp")

add_library(zetarf ${SOURCES})
target_link_libraries(${PROJECT_NAME} wiringPi)
