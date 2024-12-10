set(cvec_src 
    "cvec.c"
    "cvec_gs.c"
    "cvec.h"
)
list(TRANSFORM cvec_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(cvec_public_src
"cvec.h"
)

AddSubdir_ProjectFolder("cvecstr" "cvec_src")

get_property_and_append("cvecstr" "cvecstr_public_src" "cvec_public_src")

list(TRANSFORM cvec_public_src PREPEND "../public/cvec/")

Get_simpleheaders()

add_library(cvec "${cvec_src}")
set_target_properties(cvec PROPERTIES
PUBLIC_HEADER "${cvec_public_src}")

if (cvec_BUILD_CVECSTR)
#TODO fix this so that the optional library is actually optional to build,
#TODO not just get access to
target_compile_definitions(cvec PUBLIC "-Dcvec_BUILD_CVECSTR")
else()
target_compile_definitions(cvec PUBLIC "-Dcvec_BUILD_CVECSTR=0")
endif()

if (cvec_BUILD_NULLCHECK)
target_compile_definitions(cvec PUBLIC "-Dcvec_BUILD_NULLCHECK")
else()
target_compile_definitions(cvec PUBLIC "-Dcvec_BUILD_NULLCHECK=0")
endif()

target_include_directories(cvec PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(cvec PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../public/")

target_link_libraries(cvec PRIVATE simpleheaders)