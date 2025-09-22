cmake_minimum_required(VERSION 4.0)

# Requires C++20.
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ImageIO VERSION 1.0.0 LANGUAGES CXX)

# Search in the "cmake" directory for additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_library(ImageIO STATIC
  "BitmapImpl.h"
  "IBitmap.h"
  "loadImage.h"
  "loadJpeg.h"
  "loadPng.h"
  "readBinaryFile.h"
  "saveImage.h"
  "loadImage.cpp"
  "loadJpeg.cpp"
  "loadPng.cpp"
  "readBinaryFile.cpp"
  "saveImage.cpp"
  # lodepng recommends adding its sources directry to the project.
  "thirdparty/lodepng/lodepng.h"
  "thirdparty/lodepng/lodepng.cpp"
)
add_library(ImageIO::ImageIO ALIAS ImageIO)
target_include_directories(ImageIO PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Thirdparty libraries:
add_subdirectory(thirdparty/imageview EXCLUDE_FROM_ALL)
# libjpeg-turbo is "special", it cannot be added via add_subdirectory().
find_package(JpegTurbo REQUIRED)

target_link_libraries(ImageIO PUBLIC imageview)
target_link_libraries(ImageIO PRIVATE libjpeg-turbo)

# Set warning level
if(MSVC)
  target_compile_options(ImageIO PRIVATE /W4 /permissive-)
else()
  target_compile_options(ImageIO PRIVATE -Wall -Wextra -Wpedantic)
endif()
