cmake_minimum_required(VERSION 3.10)
set(CMAKE_INSTALL_PREFIX "bin" CACHE PATH "bin install prefix")
set (CMAKE_CXX_STANDARD 17)
project(catch VERSION 1.0 LANGUAGES C CXX)


add_library(catch_lib SHARED
	    catch.cpp)

target_include_directories(catch_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
