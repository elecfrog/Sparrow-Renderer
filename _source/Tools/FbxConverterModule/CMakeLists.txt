cmake_minimum_required(VERSION 3.17)

set(LIB_NAME "FbxConverterModule")

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIB_NAME} STATIC
        FbxSdkUtils.cpp
        FbxSdkUtils.h
        FbxScene.h
)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # MSVC runtime library flags are selected by an abstraction.
endif(POLICY CMP0091)

project(FbxConverterModule)

if(NOT COMMAND fbx_target_finalize)
    # Architecture detection
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        if("${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM64")
            SET(FBX_ARM 1)
            set(FBX_ARCH arm64) # 64 bits
        else()
            set(FBX_ARCH x64) # 64 bits
        endif()
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        if("${CMAKE_C_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM")
            SET(FBX_ARM 1)
            set(FBX_ARCH arm) # 32 bits
        else()
            set(FBX_ARCH x86) # 32 bits
        endif()
    endif()

    if(NOT FBX_CONFIGURATION)
        if(WIN32)
            set(FBX_CONFIGURATION vc${MSVC_TOOLSET_VERSION}_${FBX_ARCH}_dll)
        else()
            if(APPLE)
                set(FBX_CONFIGURATION clang_macos_dylib)
            else()
                set(FBX_CONFIGURATION gcc_${FBX_ARCH}_so_lib)
            endif()
        endif()
    endif()

    if(NOT FBX_BUILD_ROOT)
        set(FBX_BUILD_ROOT ${CMAKE_SOURCE_DIR}/../build/${FBX_CONFIGURATION})
    endif()
    IF(NOT EXISTS "${FBX_BUILD_ROOT}")
        FILE(MAKE_DIRECTORY "${FBX_BUILD_ROOT}")
    endif()

    INCLUDE("${CMAKE_SOURCE_DIR}/_external/fbxsdk/fbxsdk.cmake")
endif()

if(FBX_SHARED)
    add_compile_definitions(FBXSDK_SHARED)
endif()

# Setup Headers
target_include_directories(${LIB_NAME}
        PUBLIC
        "."
        ${CMAKE_SOURCE_DIR}/_source
)

# 链接FBX SDK
target_link_libraries(${LIB_NAME}
    PRIVATE
    Base
    Log
    fbxsdk${FBX_FILESDK_LIB_SUFFX}
)

# 设置输出目录
set_target_properties(${LIB_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# 设置IDE中的文件夹
set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Source/Tools")

# 使用FBX SDK的配置函数来设置目标
fbx_target_finalize(${LIB_NAME})