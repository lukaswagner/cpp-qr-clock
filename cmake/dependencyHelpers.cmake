# check out repo
# optional param: branch or tag
function(git dir url)
    set(source "${src_dir}/${dir}")
    if(EXISTS ${source})
        message(NOTICE "git: ${dir} already exists")
        return()
    endif()
    set(args --depth 1)
    if(ARGC GREATER 2)
        set(args ${args} --branch ${ARGV2} --config advice.detachedHead=false)
    endif()
    execute_process(
        COMMAND git clone ${args} ${url} ${source}
        COMMAND_ERROR_IS_FATAL ANY)
endfunction()

# init submodules
function(submodules dir)
    if(NOT EXISTS ${src_dir}/${dir})
        message(NOTICE "submodule: ${dir} doesn't exists")
        return()
    endif()
    execute_process(
        COMMAND git submodule init
        WORKING_DIRECTORY "${src_dir}/${dir}"
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
        COMMAND git submodule update
        WORKING_DIRECTORY "${src_dir}/${dir}"
        COMMAND_ERROR_IS_FATAL ANY)
endfunction()

# call cmake in dir
# optional param: additional configure args
function(build dir)
    set(source "${src_dir}/${dir}")
    if(NOT EXISTS ${src_dir}/${dir})
        message(NOTICE "submodule: ${dir} doesn't exists")
        return()
    endif()
    execute_process(
        COMMAND cmake ${configure_args}
        -S ${source} -B ${source}
        -DCMAKE_INSTALL_PREFIX=${dependency_dir}
        "-DCMAKE_PREFIX_PATH=${dependency_dir}"
        ${ARGV1}
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
        COMMAND cmake --build ${source} --target install
        ${build_args}
        COMMAND_ERROR_IS_FATAL ANY)
endfunction()
