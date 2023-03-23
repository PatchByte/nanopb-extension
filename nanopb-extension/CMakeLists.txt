project("nanopb-extension")

find_package(Nanopb REQUIRED)

file(GLOB NANOPB_EXTENSION_SOURCE_FILES
    "source/*.*"
    "include/nanopb-extension/*.*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/
                                    ${NANOPB_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${NANOPB_EXTENSION_SOURCE_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} protobuf-nanopb-static)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)