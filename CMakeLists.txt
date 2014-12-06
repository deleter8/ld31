cmake_minimum_required(VERSION 2.8)
##############################################
# Project Name 
##############################################
project(ld31)
##############################################
# Find packages
##############################################
find_package(PkgConfig REQUIRED)
pkg_search_module(SFML2 REQUIRED sfml-all)
##############################################
# Add include and set source 
##############################################
include_directories(include/)
aux_source_directory(src/ SRC)
##############################################
# Build Binary 
##############################################
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -g -Wextra -Wshadow -pedantic ${CMAKE_CXX_FLAGS}")
add_executable(${PROJECT_NAME}.bin ${SRC})
target_link_libraries(${PROJECT_NAME}.bin ${SFML2_LIBRARIES})

	
