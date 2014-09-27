cmake_minimum_required (VERSION 2.8.6)
project (HELLO)

# Set up include directories
include_directories (include)

# Specify output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package( OpenCV REQUIRED )

add_executable (ParticleFilter 
	src/particleFilter.cpp
	src/mapReader.cpp
	src/logReader.cpp
	src/visualize.cpp)

target_link_libraries( ParticleFilter ${OpenCV_LIBS} )