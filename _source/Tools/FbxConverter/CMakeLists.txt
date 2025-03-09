set(APP_NAME "FbxConverterApp")

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # MSVC runtime library flags are selected by an abstraction.
endif(POLICY CMP0091)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建可执行文件
add_executable(${APP_NAME}
        main.cpp
)

# 链接FbxConverterModule和FBX SDK
target_link_libraries(${APP_NAME}
    PRIVATE
    FbxConverterModule
    fbxsdk${FBX_FILESDK_LIB_SUFFX}
    Log
)

# 包含FbxConverterModule的头文件目录
target_include_directories(${APP_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/_source/FbxConverterModule
)

# 设置输出目录
set_target_properties(${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# 设置IDE中的文件夹
set_target_properties(${APP_NAME} PROPERTIES FOLDER "Source/Tools")

# 使用FBX SDK的配置函数来设置目标
fbx_target_finalize(${APP_NAME})

# 复制FBX SDK的DLL到输出目录
if(WIN32)
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/_external/fbxsdk/lib/x64/debug/libfbxsdk.dll>"
            "$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/_external/fbxsdk/lib/x64/release/libfbxsdk.dll>"
            "$<TARGET_FILE_DIR:${APP_NAME}>"
    )
endif()
