cmake_minimum_required(VERSION 3.16.3)

# Configurable compile options.
set(DEBUG           OFF)
set(LOG_TO_CONSOLE  OFF)
set(LOG_TO_FILE     OFF)
set(DUMP_STACK      OFF)     # This option needs DEBUG enabled.

configure_file(config.h.in Config.h)

# Project set up.
set(PROJECT_NAME "NES")
project(
    ${PROJECT_NAME}
    VERSION "0.1.0"
    LANGUAGES CXX
    DESCRIPTION "NES Emulator"
)
set(CMAKE_CXX_FLAGS "-std=gnu++11")

# Sources
aux_source_directory(${PROJECT_SOURCE_DIR}/src/ MAIN_SOURCES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/File/ FILE_SOURCES)
set(SOURCES
    ${SOURCES} 
    ${MAIN_SOURCES}
    ${FILE_SOURCES}
)

# Set up logging defines.
if (LOG_TO_CONSOLE OR LOG_TO_FILE OR DEBUG)
    add_definitions(-DUSE_LOGGER)
    aux_source_directory(${PROJECT_SOURCE_DIR}/src/Logger/ LOGGER_SOURCES)
    set(SOURCES
        ${SOURCES}
        ${LOGGER_SOURCES}
    )
endif()

if (DEBUG)
    message("Debug Logging Enabled.")
    add_definitions(-DDEBUG)
endif()

if (LOG_TO_CONSOLE)
    message("Console Logging Enabled.")
    add_definitions(-DSTDOUT_LOGGER)
endif()

if (LOG_TO_FILE)
    message("File Logging Enabled.")
    add_definitions(-DFILE_LOGGER)
endif()

# Includes
set(INCLUDES
    ${INCLUDES} 
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include/
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PUBLIC -g -Wall -MMD -MP -Wno-multichar)
target_include_directories(NES PUBLIC ${INCLUDES})

set_target_properties(NES PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
