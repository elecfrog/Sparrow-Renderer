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
    ${GLFW_INCLUDE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/
)



# 设置外部库的虚拟文件夹
set_target_properties(cereal PROPERTIES FOLDER "External")
set_target_properties(glfw PROPERTIES FOLDER "External")
set_target_properties(imgui PROPERTIES FOLDER "External")