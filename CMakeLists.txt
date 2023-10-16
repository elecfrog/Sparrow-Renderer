cmake_minimum_required(VERSION 3.12)

project(Sparrow-Renderer
        VERSION 0.1.0
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Setup own library path
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/external/) 

add_executable(${PROJECT_NAME}
        ${HEADER_FILES}
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# Adding opengl
find_package(OpenGL REQUIRED)                   


# cereal
if (NOT TARGET cereal)
    option(BUILD_DOC "Build documentation" OFF)
    option(BUILD_SANDBOX "Build sandbox examples" OFF)
    option(SKIP_PERFORMANCE_COMPARISON "Skip building performance sandbox comparison (requires boost)" ON)
    add_subdirectory(${EXTERNAL_PATH}/cereal)
    target_link_libraries(${PROJECT_NAME} PRIVATE cereal)
endif ()

# glfw
if (NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(${EXTERNAL_PATH}/glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif ()

# glm
if (NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory(${EXTERNAL_PATH}/glm)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif ()

# nfd
if (NOT TARGET nativefiledialog)
    add_subdirectory(${EXTERNAL_PATH}/nativefiledialog-cmake)
    target_link_libraries(${PROJECT_NAME} PRIVATE nativefiledialog)
endif ()

# spdlog
if (NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(${EXTERNAL_PATH}/spdlog)
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
endif ()

set(ASSIMP_BUILD_ZLIB ON)
# assimp
if (NOT TARGET assimp)
    option(BUILD_SHARED_LIBS OFF)
    option(ASSIMP_BUILD_FRAMEWORK OFF)
    option(ASSIMP_DOUBLE_PRECISION OFF)
    option(ASSIMP_OPT_BUILD_PACKAGES OFF)
    option(ASSIMP_ANDROID_JNIIOSYSTEM OFF)
    option(ASSIMP_NO_EXPORT OFF)
    option(ASSIMP_BUILD_ZLIB OFF)
    option(ASSIMP_COVERALLS OFF)
    option(ASSIMP_INSTALL OFF)
    option(ASSIMP_ERROR_MAX OFF)
    option(ASSIMP_ASAN OFF)
    option(ASSIMP_UBSAN OFF)
    option(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
    option(ASSIMP_IGNORE_GIT_HASH OFF)
    add_subdirectory(${EXTERNAL_PATH}/assimp)
    target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
endif ()

# Adding imgui
add_library(imgui
        # static library source
        STATIC
        ${EXTERNAL_PATH}/imgui/imgui.cpp
        ${EXTERNAL_PATH}/imgui/imgui_demo.cpp
        ${EXTERNAL_PATH}/imgui/imgui_draw.cpp
        ${EXTERNAL_PATH}/imgui/imgui_widgets.cpp
        ${EXTERNAL_PATH}/imgui/imgui_tables.cpp
        ${EXTERNAL_PATH}/imgui/backends/imgui_impl_opengl3.cpp
        ${EXTERNAL_PATH}/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui
        # static library headers
        PRIVATE
        ${EXTERNAL_PATH}/imgui
        ${EXTERNAL_PATH}/glfw/include/        # glfw-binding needs to include glfw codes
)

# Setup OpenFBX
add_library(OpenFBX                             # static library source
        STATIC
        ${EXTERNAL_PATH}/OpenFBX/miniz.c
        ${EXTERNAL_PATH}/OpenFBX/ofbx.cpp
)

target_include_directories(OpenFBX
        PUBLIC
        ${EXTERNAL_PATH}/
)


# Apply dependencies
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        imgui
        OpenFBX
)

# Setup Headers
include_directories(
        SYSTEM
        ${CMAKE_SOURCE_DIR}/              # include All headers needed
        ${CMAKE_SOURCE_DIR}/src/          # include All headers needed
        ${EXTERNAL_PATH}/glad/include     # include glad.h as the loading header
        ${EXTERNAL_PATH}/tinyobjloader/   # include tiny_obj_loader.h
        ${EXTERNAL_PATH}/stb              # include stb
        ${EXTERNAL_PATH}/imgui            # include imgui
        ${EXTERNAL_PATH}/assimp/include   # include assimp
        ${EXTERNAL_PATH}/cereal/include   # include cereal
        ${EXTERNAL_PATH}/nativefiledialog-cmake/src/include   # include nfd
        ${SPDLOG_INCLUDE_DIRS}
)

# Deal with resource dependencies (of the executable)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}") # Not the best practice, but it works for now