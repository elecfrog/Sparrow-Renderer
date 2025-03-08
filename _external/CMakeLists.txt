# cereal
if (NOT TARGET cereal)
    option(BUILD_DOC "Build documentation" OFF)
    option(BUILD_SANDBOX "Build sandbox examples" OFF)
    option(SKIP_PERFORMANCE_COMPARISON "Skip building performance sandbox comparison (requires boost)" ON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cereal)
endif()

# glfw
if (NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw)
endif()

# glm
if (NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glm)
endif()

# nfd
if (NOT TARGET nativefiledialog)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog-cmake)
endif()

# spdlog
if (NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
endif()

# assimp
if (NOT TARGET assimp)
    option(BUILD_SHARED_LIBS OFF)
    option(ASSIMP_BUILD_FRAMEWORK OFF)
    option(ASSIMP_DOUBLE_PRECISION OFF)
    option(ASSIMP_OPT_BUILD_PACKAGES OFF)
    option(ASSIMP_ANDROID_JNIIOSYSTEM OFF)
    option(ASSIMP_NO_EXPORT OFF)
    option(ASSIMP_BUILD_ZLIB ON)
    option(ASSIMP_COVERALLS OFF)
    option(ASSIMP_INSTALL OFF)
    option(ASSIMP_ERROR_MAX OFF)
    option(ASSIMP_ASAN OFF)
    option(ASSIMP_UBSAN OFF)
    option(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
    option(ASSIMP_IGNORE_GIT_HASH OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/assimp)
endif()

# imgui
add_library(imgui
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/
)

# OpenFBX
add_library(OpenFBX
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenFBX/miniz.c
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenFBX/ofbx.cpp
)

target_include_directories(OpenFBX
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

# 设置外部库的虚拟文件夹
set_target_properties(cereal PROPERTIES FOLDER "External")
set_target_properties(glfw PROPERTIES FOLDER "External")
set_target_properties(glm PROPERTIES FOLDER "External")
set_target_properties(nativefiledialog PROPERTIES FOLDER "External")
set_target_properties(spdlog PROPERTIES FOLDER "External")
set_target_properties(assimp PROPERTIES FOLDER "External")
set_target_properties(imgui PROPERTIES FOLDER "External")
set_target_properties(OpenFBX PROPERTIES FOLDER "External")