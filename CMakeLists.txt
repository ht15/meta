cmake_minimum_required(VERSION 3.8)
PROJECT(meta)
#dependency

if (UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

if (WIN32)
        set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
elseif (APPLE)
        message(STATUS "Compile in Apple System.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC -w -pipe -fPIE")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -fpic -pipe")
        add_definitions(-g)
        add_definitions(-O0)
elseif (LINUX)
        message(STATUS "Compile in Linux System.")
	# -D_GLIBCXX_USE_CXX11_ABI=0
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -fPIC -w -pipe -mcmodel=medium -fPIE -Wl,--no-keep-memory")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -fpic -pipe -Wl,--no-keep-memory")
        add_definitions(-g)
        add_definitions(-O0)
else ()
        message(STATUS "unknow System!!")
endif ()

include(third_party/thirdparty.cmake)

add_subdirectory(core)

# add_subdirectory(test)

add_subdirectory(example)





