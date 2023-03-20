cmake_minimum_required(VERSION 3.24)
project("nanopb-extension-build")

option(NANOPB_EXTENSION_AS_VENDOR "Enables the Nanopb extension project to be used as a vendor." OFF)
option(NANOPB_EXTENSION_BUILD_TESTS "Enables the Nanopb extension tests." ON)
option(NANOPB_EXTENSION_CMAKE_MODULE_PATH "Nanopb CMake module path." ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/)

if(NOT NANOPB_EXTENSION_AS_VENDOR)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(NOT NANOPB_EXTENSION_AS_VENDOR)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("nanopb-extension")