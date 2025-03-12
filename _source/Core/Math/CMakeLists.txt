set(LIB_NAME "Math")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIB_NAME} STATIC
        Exp.hpp
        GaussianKernel.hpp
        Maths.cpp
        Maths.h
        MathUtils.cpp
        MathUtils.h
        Plane.cpp
        Plane.h
        Pow.hpp
        Transform.cpp
        Transform.h
        Vector2.h
        Vector3.h
        Vector4.h
        QuaternionW.h
        Matrix4x4.h
)

target_include_directories(${LIB_NAME}
        PUBLIC
        .
        ${CMAKE_SOURCE_DIR}/_external/glm
)

find_package(glm CONFIG REQUIRED)

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