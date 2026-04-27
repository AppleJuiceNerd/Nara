cmake_minimum_required(VERSION 3.10.0)
project(nara VERSION 0.0.1 LANGUAGES C CXX)


add_subdirectory(hidapi)
include_directories(hidapi/hidapi)

add_library(${PROJECT_NAME}
	src/LL/commands.cpp
	
	src/HL/nara.cpp
	src/HL/sayo.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE hidapi::hidapi)