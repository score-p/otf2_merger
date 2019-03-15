cmake_minimum_required(VERSION 3.10)
set(CMAKE_INSTALL_PREFIX "bin" CACHE PATH "bin install prefix")
set (CMAKE_CXX_STANDARD 17)
project(test VERSION 1.0 LANGUAGES C CXX)

add_executable(otf2Merger-tests
   testIOFileProps.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../src/definition_store.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../src/maps.cpp)

target_include_directories(otf2Merger-tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src/include>)

target_link_libraries(otf2Merger-tests
   PUBLIC otf2::otf2 PUBLIC catch_lib)

