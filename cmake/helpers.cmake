function(addDep dep)
    add_custom_command(TARGET clock POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dep_dir}/${dep}.dll" $<TARGET_FILE_DIR:clock>)
endfunction()
