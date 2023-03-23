cmake_minimum_required(VERSION 3.24)
project("nanopb-extension-build")

option(NANOPB_EXTENSION_AS_VENDOR "Enables the Nanopb extension project to be used as a vendor." OFF)
option(NANOPB_EXTENSION_BUILD_TESTS "Enables the Nanopb extension tests." ON)

if(NOT NANOPB_EXTENSION_AS_VENDOR)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(NOT NANOPB_EXTENSION_AS_VENDOR)

set(CMAKE_CXX_STANDARD 20)

if(NOT NANOPB_EXTENSION_AS_VENDOR)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Vendors/nanopb/extra)
    add_subdirectory("Vendors")
endif(NOT NANOPB_EXTENSION_AS_VENDOR)

add_subdirectory("nanopb-extension")

if(NANOPB_EXTENSION_BUILD_TESTS)
    enable_testing()
    add_subdirectory("nanopb-extension-tests")
endif(NANOPB_EXTENSION_BUILD_TESTS)