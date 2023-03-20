project("nanopb-extension")

file(GLOB NANOPB_EXTENSION_SOURCE_FILES
    "source/*.*"
    "include/nanopb-extension/*.*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_executable(${PROJECT_NAME} ${NANOPB_EXTENSION_SOURCE_FILES})