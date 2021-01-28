cmake_minimum_required(VERSION 3.13.0)
project(resize_images)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -m64")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif()

set(SOURCE_FILES src/main.cpp)
add_executable(resize_images ${SOURCE_FILES})

target_link_libraries(resize_images ${CONAN_LIBS})