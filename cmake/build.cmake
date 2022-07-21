cmake_minimum_required(VERSION 3.19)

if(NOT EXISTS CMakeLists.txt)
    message(FATAL_ERROR "Please run this from the root directory")
endif()

include(cmake/config.cmake)
file(MAKE_DIRECTORY ${build_dir})

execute_process(
    COMMAND cmake ${configure_args}
    -S . -B ${build_dir}
    "-DCMAKE_PREFIX_PATH=${dependency_dir}"
    COMMAND_ERROR_IS_FATAL ANY)
execute_process(
    COMMAND cmake --build ${build_dir}
    ${build_args}
    COMMAND_ERROR_IS_FATAL ANY)
