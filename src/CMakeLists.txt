cmake_minimum_required(VERSION 3.12)
project(zetarf)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.h" "*.cpp")

add_library(zetarf ${SOURCES})
target_link_libraries(${PROJECT_NAME} wiringPi)
