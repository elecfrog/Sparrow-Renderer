set(APP_NAME "Application")

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # MSVC runtime library flags are selected by an abstraction.
endif(POLICY CMP0091)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建可执行文件
add_executable(${APP_NAME}
    Application.hpp
    Application.cpp
    Scene.hpp
    SceneManager.hpp
    ScenePreCompiled.h
)

find_package(nfd CONFIG REQUIRED)
#find_package(cereal CONFIG REQUIRED)



# 链接FbxConverterModule和FBX SDK
target_link_libraries(${APP_NAME}
    PRIVATE
    Base
    Log
    Math
    Pattern
    Client
    nfd::nfd
#    cereal::cereal
)

# 设置输出目录
set_target_properties(${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# 设置IDE中的文件夹
set_target_properties(${APP_NAME} PROPERTIES FOLDER "Source/Application")