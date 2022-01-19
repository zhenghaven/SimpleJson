# Copyright 2022 Haofan Zheng
# See accompanying file LICENSE

cmake_minimum_required(VERSION 3.14)

project(SimpleJson VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(include)

OPTION(SIMPLEJSON_TEST "Option to build SimpleJson test executable." OFF)
OPTION(SIMPLEJSON_TEST_LCOV "Option to turn on test code coverage." OFF)
# SET(SIMPLEJSON_TEST ON CACHE BOOL "Option to build SimpleJson test executable." FORCE)

set(ENV{SIMPLEJSON_HOME} ${CMAKE_CURRENT_LIST_DIR})
set(SIMPLEJSON_HOME ${CMAKE_CURRENT_LIST_DIR})

set(ENV{SIMPLEJSON_INCLUDE} $ENV{SIMPLEJSON_HOME}/include)
set(SIMPLEJSON_INCLUDE ${SIMPLEJSON_HOME}/include)

add_library(SimpleJson INTERFACE)

target_include_directories(SimpleJson INTERFACE include)

if(${SIMPLEJSON_TEST})
	enable_testing()
	add_subdirectory(test)
endif(${SIMPLEJSON_TEST})
