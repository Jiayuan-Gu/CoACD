cmake_minimum_required(VERSION 3.1)
project(CoACD)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -Wall -pthread")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-int-in-bool-context -Wsign-compare")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("-- CMAKE_BUILD_TYPE Debug")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
else()
    message("-- CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
endif()

include_directories(src)
include_directories(3rd/glm)
include_directories(3rd/eigen/)
include_directories(3rd/libigl/include)

file(GLOB_RECURSE MAIN_SRC "src/*.h" "src/*.cc" "src/*.hpp" "src/*.cpp")
add_library(coacd SHARED
    ${MAIN_SRC}
    src/manifold/ManifoldPlus.h
)

add_executable(main main.cpp)
target_link_libraries(main coacd)
