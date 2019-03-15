cmake_minimum_required(VERSION 3.10)
set(CMAKE_INSTALL_PREFIX "bin" CACHE PATH "bin install prefix")
set (CMAKE_CXX_STANDARD 17)
project(otf2_merger VERSION 1.0 LANGUAGES C CXX)

# Set a default build type if none was specified
set(default_build_type "Debug")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(EXECUTABLE_OUTPUT_PATH "../bin")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(OTF2 8.0.1 EXACT REQUIRED)

add_subdirectory(thirdParty)
add_subdirectory(src) 
add_subdirectory(test EXCLUDE_FROM_ALL)

