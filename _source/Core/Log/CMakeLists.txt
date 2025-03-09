set(LIB_NAME "Log")

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建库
add_library(${LIB_NAME} STATIC
        LogSystem.cpp
        LogSystem.h
)

# 设置包含目录
target_include_directories(${LIB_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/_source
)

# 链接spdlog
target_link_libraries(${LIB_NAME}
        PUBLIC
        spdlog::spdlog
)

# 设置输出目录
set_target_properties(${LIB_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# 设置IDE中的文件夹
set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Source/Core") 