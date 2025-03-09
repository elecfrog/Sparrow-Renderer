cmake_minimum_required(VERSION 3.20)

project(Sparrow-Renderer
        VERSION 0.5.0
        LANGUAGES C CXX
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置CMake策略
cmake_policy(SET CMP0079 NEW)

# 设置外部依赖路径
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/_external/)

# 创建虚拟文件夹
source_group("External Libraries" FILES ${EXTERNAL_PATH}/*)
#source_group("External Libraries\\cereal" FILES ${EXTERNAL_PATH}/cereal/*)
source_group("External Libraries\\glfw" FILES ${EXTERNAL_PATH}/glfw/*)
source_group("External Libraries\\glm" FILES ${EXTERNAL_PATH}/glm/*)
source_group("External Libraries\\nativefiledialog" FILES ${EXTERNAL_PATH}/nativefiledialog-cmake/*)
#source_group("External Libraries\\spdlog" FILES ${EXTERNAL_PATH}/spdlog/*)

source_group("External Libraries\\imgui" FILES ${EXTERNAL_PATH}/imgui/*)
source_group("External Libraries\\OpenFBX" FILES ${EXTERNAL_PATH}/OpenFBX/*)

#find_package(zlib REQUIRED)
find_package(assimp CONFIG REQUIRED)
#find_package(cereal CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)

# 添加外部依赖
add_subdirectory(_external)

# 添加基础库
add_subdirectory(_source/Base)
add_subdirectory(_source/Log)
add_subdirectory(_source/Math)
add_subdirectory(_source/Client)
add_subdirectory(_source/Application)
add_subdirectory(_source/Pattern)
add_subdirectory(_source/FbxConverterModule)
add_subdirectory(_source/FbxConverter)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置IDE中的文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)