function(addDep dep)
    add_custom_command(TARGET deferred POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dep_dir}/bin/${dep}.dll" $<TARGET_FILE_DIR:deferred>)
endfunction()
