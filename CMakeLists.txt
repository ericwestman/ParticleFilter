cmake_minimum_required (VERSION 2.8.11)
project (HELLO)

# Set up include directories
include_directories (include)

# Specify output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable (ParticleFilter 
	src/particleFilter.cpp)

#target_link_libraries (helloDemo LINK_PUBLIC Hello)