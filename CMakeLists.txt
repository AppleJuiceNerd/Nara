cmake_minimum_required(VERSION 3.10.0)
project(nara VERSION 0.0.1 LANGUAGES C CXX)


add_subdirectory(hidapi)
include_directories(hidapi/hidapi)

add_library(${PROJECT_NAME}
	src/LL/packet.cpp
	src/LL/package_base.cpp
	src/LL/light_data.cpp
	src/LL/commands.cpp
	src/LL/boxcutter.cpp
	src/naraapi_hl.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE hidapi::hidapi)