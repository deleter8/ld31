cmake_minimum_required(VERSION 2.8)
##############################################
# Project Name 
##############################################
project(resource_loader_example)
##############################################
# Find packages
##############################################
##############################################
# Add include and set source 
##############################################
set (SRC
	main.cc
)
##############################################
# Build Binary 
##############################################
#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -g -Wl,--no-as-needed -pthread ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -g -pthread ${CMAKE_CXX_FLAGS}")
add_executable(${PROJECT_NAME}.bin ${SRC})
target_link_libraries(${PROJECT_NAME}.bin )

