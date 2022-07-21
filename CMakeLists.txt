cmake_minimum_required(VERSION 3.0.0)

# project settings
project(clock CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0057 NEW)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# dependencies
set(dep_dir ${CMAKE_SOURCE_DIR}/dependencies)

set(CMAKE_PREFIX_PATH
    "${dep_dir}" ${CMAKE_PREFIX_PATH})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(glbinding REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${dep_dir}/include)

# sources
set(source_dir src)
set(header_dir include)

set(sources
    ${source_dir}/main.cpp)

# executables
add_executable(clock ${sources})
target_include_directories(clock PRIVATE ${header_dir})
target_link_libraries(clock PRIVATE glfw glbinding::glbinding)

# dlls for runtime
if(WIN32)
    include(cmake/helpers.cmake)
endif()
