cmake_minimum_required(VERSION 3.16.3)

set(PROJECT_NAME "NES")
project(
    ${PROJECT_NAME}
    VERSION "0.1.0"
    LANGUAGES CXX
    DESCRIPTION "NES Emulator"
)
set(CMAKE_CXX_FLAGS "-std=gnu++11")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ MAIN_SOURCES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/File/ FILE_SOURCES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Logger/ LOGGER_SOURCES)

configure_file(config.h.in Config.h)

set(SOURCES
    ${SOURCES} 
    ${MAIN_SOURCES}
    ${FILE_SOURCES}
    ${LOGGER_SOURCES}
)

set(INCLUDES
    ${INCLUDES} 
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include/
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PUBLIC -g -Wall -MMD -MP -Wno-multichar)

target_include_directories(NES PUBLIC
    ${INCLUDES}
)

set_target_properties(NES PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
