cmake_minimum_required(VERSION 3.10.0)
project(nara VERSION 0.0.1 LANGUAGES C CXX)


add_subdirectory(hidapi)
include_directories(hidapi/hidapi)

add_library(${PROJECT_NAME}
	src/naraapi_hl.cpp
	src/naraapi_ll.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE hidapi::hidapi)