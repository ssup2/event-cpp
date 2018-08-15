cmake_minimum_required (VERSION 2.6)

# - Project -
set(CMAKE_CXX_COMPILER "clang++")
project (event-cpp)

# - Variables -
## Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 5)

# - Compiler -
## Setting
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-limit-debug-info -g -Wall")

## Include
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/framework) 

# - Build framework and copy it's headers -
add_custom_target(framework
    DEPENDS framework_lib
    DEPENDS framework_header
    )

## Build framework library
add_library(framework_lib STATIC
    EXCLUDE_FROM_ALL
	framework/event/handler.cc
	framework/event/manager.cc
	framework/helper/knock.cc
	framework/helper/signal.cc
	framework/helper/socket.cc
	framework/helper/timer.cc
	framework/logger/logger.cc
    )

set_target_properties(framework_lib PROPERTIES OUTPUT_NAME "eventcpp")	
set_target_properties(framework_lib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/framework)	

## Copy framework's headers
add_custom_command(OUTPUT framework_header
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/framework
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/framework/event
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/event/event.h ${CMAKE_CURRENT_BINARY_DIR}/framework/event
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/event/manager.h ${CMAKE_CURRENT_BINARY_DIR}/framework/event
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/event/handler.h ${CMAKE_CURRENT_BINARY_DIR}/framework/event
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/framework/helper
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/helper/knock.h ${CMAKE_CURRENT_BINARY_DIR}/framework/helper
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/helper/signal.h ${CMAKE_CURRENT_BINARY_DIR}/framework/helper
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/helper/socket.h ${CMAKE_CURRENT_BINARY_DIR}/framework/helper
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/helper/timer.h ${CMAKE_CURRENT_BINARY_DIR}/framework/helper
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/framework/logger
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/framework/logger/logger.h ${CMAKE_CURRENT_BINARY_DIR}/framework/logger
    ) 

