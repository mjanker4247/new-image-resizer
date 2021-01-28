cmake_minimum_required(VERSION 3.13.0)
project(resize_images)

#add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES src/main.cpp)
add_executable(resize_images ${SOURCE_FILES})

target_link_libraries(resize_images ${CONAN_LIBS})