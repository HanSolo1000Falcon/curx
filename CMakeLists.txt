cmake_minimum_required(VERSION 4.1)

project(curx LANGUAGES C)
find_package(CURL REQUIRED)
find_package(cJSON REQUIRED)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(curx src/main.c)
target_include_directories(curx PRIVATE include)
target_link_libraries(curx PRIVATE CURL::libcurl cjson)
