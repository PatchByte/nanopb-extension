project("nanopb-extension-test")

find_package(Nanopb REQUIRED)

file(GLOB NANOPB_EXTENSION_TEST_SOURCE_FILES
    "include/*.*"
    "source/*.*"

    "proto/*.c"
    "proto/*.h"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/
                                    ${CMAKE_CURRENT_SOURCE_DIR}/proto/
                                    ${NANOPB_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${NANOPB_EXTENSION_TEST_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} protobuf-nanopb-static nanopb-extension::nanopb-extension)