cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
project(src VERSION 1.0 LANGUAGES C CXX)

add_executable(otf2-merger
  main.cpp
  config.cpp 
  definition_reader.cpp
  definition_store.cpp
  maps.cpp
  definition_writer.cpp
  event_processor.cpp)

target_link_libraries(otf2-merger 
   PUBLIC otf2::otf2)

target_include_directories(otf2-merger PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_options(otf2-merger PRIVATE -Wall -Wno-pedantic -Werror -Wextra -Wno-unused-function) 
