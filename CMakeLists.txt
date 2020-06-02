cmake_minimum_required(VERSION 3.6)

project(AsioChatRoom)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(architecture x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(architecture x86)
endif()

if(WIN32)
    set(os windows)
elseif(APPLE)
    set(os apple)
else()
    set(os linux)
endif()

if(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
endif()

add_subdirectory(source)
add_subdirectory(apps)