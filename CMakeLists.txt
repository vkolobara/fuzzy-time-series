cmake_minimum_required(VERSION 3.8)

project(fuzzy_time_series)
set(fuzzy_time_series_VERSION_MAJOR 1)
set(fuzzy_time_series_VERSION_MINOR 0)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/win32libs/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/win32libs/boost/lib")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp)

add_subdirectory(lib/ECF)
add_subdirectory(lib/fuzzy-inference-system)

add_executable(fuzzy_time_series src/main.cpp)