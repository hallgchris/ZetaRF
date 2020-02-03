cmake_minimum_required(VERSION 3.12)
project(zetarf)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/**/*.h" "src/**/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} wiringPi)


add_subdirectory(examples/SimpleRxTx)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND python3 setup.py build_ext --inplace
	COMMAND rm -r zetarf433.cpp build/temp.*
)
