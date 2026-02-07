cmake_minimum_required(VERSION 3.15)

project(stdc VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Allow user to choose build type (Debug, Release, RelWithDebInfo, MinSizeRel) 
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE) 
endif()

# Collect all .c files in src/
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Define include folder
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Install library and headers into project/lib
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/lib)

