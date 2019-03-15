cmake_minimum_required(VERSION 3.10)
set(CMAKE_INSTALL_PREFIX "bin" CACHE PATH "bin install prefix")
set (CMAKE_CXX_STANDARD 17)
project(thirdParty VERSION 1.0 LANGUAGES C CXX)


add_subdirectory(catch)
