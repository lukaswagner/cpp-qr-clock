# edit the build settings here
set(config RelWithDebInfo
    CACHE STRING "Build configuration. See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html / https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html")
set(win_comp "Visual Studio 16 2019"
    CACHE STRING "Compiler used on Windows.")
set(win_arch x64
    CACHE STRING "Architecture used on Windows, required for MSVC.")
set(unix_comp "Unix Makefiles"
    CACHE STRING "Compiler used on Linux.")
set(parallel 8
    CACHE STRING "Number of parallel jobs for compilation.")
set(dependency_dir dependencies
    CACHE STRING "Directory for storing the dependencies.")
set(build_dir build
    CACHE STRING "Directory for building the project.")

if(WIN32)
    set(configure_args -G ${win_comp} -A ${win_arch})
    set(build_args --config ${config} --parallel ${parallel})
else()
    set(configure_args -G ${unix_comp} -DCMAKE_BUILD_TYPE=${config})
    set(build_args --parallel ${parallel})
endif()
