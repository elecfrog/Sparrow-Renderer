cmake_minimum_required(VERSION 3.20)

project(Sparrow-Renderer
        VERSION 0.5.0
        LANGUAGES C CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


cmake_policy(SET CMP0079 NEW)

set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/_external/)

find_package(assimp CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory(_external)
add_subdirectory(_source)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib$<CONFIG>)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)