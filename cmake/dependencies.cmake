cmake_minimum_required(VERSION 3.19)

if(NOT EXISTS CMakeLists.txt)
    message(FATAL_ERROR "Please run this from the root directory")
endif()

include(cmake/config.cmake)

get_filename_component(dependency_dir ${dependency_dir} REALPATH)
set(src_dir "${dependency_dir}/src")

# helper functions
include(cmake/dependencyHelpers.cmake)

# specify dependencies
if(NOT EXISTS ${dependency_dir}/include/glbinding)
    message(STATUS "Building glbinding")
    git(glbinding https://github.com/cginternals/glbinding.git v3.1.0)
    build(glbinding)
else()
    message(STATUS "glbinding already exists")
endif()

if(NOT EXISTS ${dependency_dir}/include/GLFW)
    message(STATUS "Building GLFW")
    git(glfw https://github.com/glfw/glfw.git 3.3.7)
    build(glfw)
else()
    message(STATUS "GLFW already exists")
endif()

# clean up
file(REMOVE_RECURSE ${src_dir})
