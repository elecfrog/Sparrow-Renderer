set(LIB_NAME "Client")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIB_NAME} STATIC
        Render/Light.h
        Render/Material.cpp
        Render/Material.h
        Render/Mesh.cpp
        Render/Mesh.h
        Render/MeshRenderer.h
        Render/Model.cpp
        Render/Model.h
        Render/Vertex.h
        Render/RenderDefinitions.h
        Render/Renderer.hpp
        Render/Skin.h
        Render/Texture2D.cpp
        Render/Texture2D.h
        Render/TextureCube.cpp
        Render/TextureCube.h
        Render/Asset.h
        Render/AssetDataSupport.inl
        Render/ResourceManager.h
        Render/StaticMesh.h
        Render/RHI/OpenGL.h
        Render/RHI/Shader.cpp
        Render/RHI/Shader.h
        Render/RHI/FrameBuffer.cpp
        Render/RHI/FrameBuffer.h
        Render/RHI/VertexArray.hpp
        Render/RHI/VertexBuffer.cpp
        Render/RHI/VertexBuffer.h
        Render/RHI/VertexBufferLayout.cpp
        Render/RHI/VertexBufferLayout.h

        Component/TransformComponent.cpp
        Component/TransformComponent.h
        Component/CameraComponent.cpp
        Component/CameraComponent.h
        Component/MouseComponent.cpp
        Component/MouseComponent.h

        FileSystem/FileDefinitions.h
        FileSystem/FileSystem.cpp
        FileSystem/FileSystem.h

        WindowSystem/WindowSystem.cpp
        WindowSystem/WindowSystem.h

        ImGui/ImGuiManager.h
        ImGui/ImGuiDockSpace.h
        ImGui/ImGuiDefinitions.h
        ImGui/ImGuiPanels/ImGuiInspectorPanel.hpp
        ImGui/ImGuiPanels/ImGuiMenuBar.hpp
        ImGui/ImGuiPanels/ImGuiObjectPanel.hpp
        ImGui/ImGuiPanels/ImGuiPanel.hpp
        ImGui/ImGuiPanels/ImGuiTreeNodePanel.hpp

        ${CMAKE_SOURCE_DIR}/_external/glad/src/glad.c
)

find_package(OpenGL REQUIRED)
find_package(nfd CONFIG REQUIRED)

target_include_directories(${LIB_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/_external/glad/include
        ${CMAKE_SOURCE_DIR}/_external/glfw
        ${CMAKE_SOURCE_DIR}/_external/imgui
)

target_link_libraries(${LIB_NAME}
        PUBLIC
        Base
        Log
        Math
        Pattern
        glm::glm
        OpenGL::GL
        glfw
        cereal
        imgui
        nfd::nfd
)

set_target_properties(${LIB_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "Source/Client") 