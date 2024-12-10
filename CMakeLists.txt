cmake_minimum_required(VERSION 3.2)
project("cvec" LANGUAGES C)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(common)

option(cvec_BUILD_CVECSTR "Build the accompanying expandable string library" on)
option(cvec_BUILD_NULLCHECK "Build with NULL deref checking" on)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED on)

if(MSVC_VERSION GREATER 1399)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif()
add_subdirectory(src)
add_subdirectory(examples)