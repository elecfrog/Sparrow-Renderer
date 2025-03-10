set(LIB_NAME "Core")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIB_NAME} INTERFACE)

target_sources(${LIB_NAME}
        INTERFACE
        Core.h
)

target_include_directories(${LIB_NAME}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${LIB_NAME}
        INTERFACE
        Base
        Log
        Math
        Pattern
)

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Source/Core")

# Add subdirectories
add_subdirectory(Base)
add_subdirectory(Log)
add_subdirectory(Math)
add_subdirectory(Pattern)
