# 添加子目录
add_subdirectory(Base)
add_subdirectory(Math)
add_subdirectory(Core)
add_subdirectory(Client)
add_subdirectory(Log)
add_subdirectory(Application)
add_subdirectory(FbxConverterModule)
add_subdirectory(FbxConverter)

# 收集所有源文件
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 创建虚拟文件夹
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files\\Core" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp)
source_group("Source Files\\Function" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Function/*.cpp)
source_group("Source Files\\Platform" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.cpp)
source_group("Source Files\\Resource" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/*.cpp)
source_group("Source Files\\Tests" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp)
source_group("Header Files\\Core" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.hpp)
source_group("Header Files\\Function" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Function/*.hpp)
source_group("Header Files\\Platform" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.hpp)
source_group("Header Files\\Resource" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/*.hpp)
source_group("Header Files\\Tests" FILES ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.hpp)

# 创建主程序
add_executable(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${CMAKE_SOURCE_DIR}/_external/glad/src/glad.c
)

# 设置C++标准
set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 设置源文件的虚拟文件夹
set_target_properties(${PROJECT_NAME} PROPERTIES 
    FOLDER "Source"
)

# 查找OpenGL
find_package(OpenGL REQUIRED)

# 设置包含目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/_external/glad/include
    ${CMAKE_SOURCE_DIR}/_external/tinyobjloader
    ${CMAKE_SOURCE_DIR}/_external/stb
    ${CMAKE_SOURCE_DIR}/_external/imgui
    ${CMAKE_SOURCE_DIR}/_external/assimp/include
    ${CMAKE_SOURCE_DIR}/_external/cereal/include
    ${CMAKE_SOURCE_DIR}/_external/nativefiledialog-cmake/src/include
)

# 链接依赖库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    OpenFBX
    FbxConverterModule
    Log
    Pattern
    glm::glm
    OpenGL::GL
    glfw
    cereal
    imgui
    nativefiledialog
    spdlog::spdlog
    assimp::assimp
)

set_target_properties(FbxConverterModule PROPERTIES FOLDER "Source")
set_target_properties(Log PROPERTIES FOLDER "Source")