set(LIB_NAME "Base")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIB_NAME} STATIC
    BaseDefinition.cpp
    BaseDefinition.h
)

target_include_directories(${LIB_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/_external/glm
)

target_link_libraries(${LIB_NAME}
    PUBLIC 
    glm::glm
) 

set_target_properties(${LIB_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Source/Core") 