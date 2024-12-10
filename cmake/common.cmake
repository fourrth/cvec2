# Gets simpleheaders repo at 'https://github.com/fourrth/Simple-Headers.git' with FetchContent
macro(Get_simpleheaders)
    include(FetchContent)
    set(simpleheaders_BUILD_EXAMPLES off)
    FetchContent_Declare(simpleheaders
        GIT_REPOSITORY git@github.com:fourrth/Simple-Headers.git
        FIND_PACKAGE_ARGS NAMES simpleheaders
        GIT_TAG v1.0.0
    )
    FetchContent_MakeAvailable(simpleheaders)
    if(NOT simpleheaders_POPULATED)
        FetchContent_Populate(simpleheaders)
        add_subdirectory(${simpleheaders_SOURCE_DIR} ${simpleheaders_BINARY_DIR})
    endif()
endmacro()

function(get_property_and_append directory directory_definition outvar)
    get_directory_property(output
    DIRECTORY ${directory}
    DEFINITION ${directory_definition}
    )
    set(ans ${${outvar}})
    list(APPEND ans ${output})
    set(${outvar} ${ans} PARENT_SCOPE)
endfunction()

macro(AddSubdir_ProjectFolder varname appendvar)
    add_subdirectory("${varname}")
    get_property_and_append("./${varname}" ${varname}_src ${appendvar})
endmacro()

macro(add_executable_example example_name)
    add_executable("${PROJECT_NAME}_Example_${example_name}" "${${example_name}_src}")
    target_link_libraries("${PROJECT_NAME}_Example_${example_name}" "cvec")
    # assumes cvec
endmacro()