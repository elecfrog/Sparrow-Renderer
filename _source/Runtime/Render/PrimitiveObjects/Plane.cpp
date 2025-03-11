/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Plane.hpp"

#include "Component/TransformComponent.h"
#include "Engine/Engine.h"
#include "System/MeshSystem.h"

namespace Sparrow
{
    void Plane::BuildMeshComponent()
    {
        MeshComponent component;
        component.m_PositionBuffer  = {
            Vector3f(-2.0f, 0.0f, 2.0f),   // 第 0 个顶点
            Vector3f(-2.0f, 0.0f, -2.0f),  // 第 1 个顶点
            Vector3f(2.0f, 0.0f, 2.0f),    // 第 2 个顶点
            Vector3f(2.0f, 0.0f, 2.0f),    // 第 3 个顶点（重复）
            Vector3f(-2.0f, 0.0f, -2.0f),  // 第 4 个顶点（重复）
            Vector3f(2.0f, 0.0f, -2.0f)    // 第 5 个顶点
        };
        component.m_NormalsBuffer   = {
            Vector3f(0.0f, 1.0f, 0.0f),    // 第 0 个顶点
            Vector3f(0.0f, 1.0f, 0.0f),    // 第 1 个顶点
            Vector3f(0.0f, 1.0f, 0.0f),    // 第 2 个顶点
            Vector3f(0.0f, 1.0f, 0.0f),    // 第 3 个顶点（重复）
            Vector3f(0.0f, 1.0f, 0.0f),    // 第 4 个顶点（重复）
            Vector3f(0.0f, 1.0f, 0.0f)     // 第 5 个顶点
        };;
        component.m_TangentsBuffer   = {
            Vector3f(0.f),    // 第 0 个顶点
            Vector3f(0.f),    // 第 1 个顶点
            Vector3f(0.f),    // 第 2 个顶点
            Vector3f(0.f),    // 第 3 个顶点（重复）
            Vector3f(0.f),    // 第 4 个顶点（重复）
            Vector3f(0.f)     // 第 5 个顶点
        };
        component.m_TexCoord0Buffer  = {
            Vector2f(1.0f, 0.0f),    // 第 0 个顶点
            Vector2f(1.0f, 1.0f),    // 第 1 个顶点
            Vector2f(0.0f, 0.0f),    // 第 2 个顶点
            Vector2f(0.0f, 0.0f),    // 第 3 个顶点（重复）
            Vector2f(1.0f, 1.0f),    // 第 4 个顶点（重复）
            Vector2f(0.0f, 1.0f)     // 第 5 个顶点
        };

        component.m_IndicesBuffer    = {
            0, 1, 2,   // 第一个三角形
            3, 4, 5    // 第二个三角形
        };

        component.m_IndexCount  = component.m_IndicesBuffer.size();
        component.m_VertexCount = component.m_PositionBuffer.size();

        g_Engine.m_MeshSystem->AddComponent(m_EntityId, std::move(component));
        m_MeshComponent = g_Engine.m_MeshSystem->GetComponent(m_EntityId);
    }

    void Plane::BuildMeshRendererComponent(SharedPtr<Shader> shader)
    {
        MeshRendererComponent renderer_component(*m_MeshComponent, shader);
        // renderer_component.m_Shader = shader;
        // renderer_component.m_MeshComponent =  m_MeshComponent;
        g_Engine.m_MeshRenderSystem->AddComponent(m_EntityId, std::move(renderer_component));
        m_MeshRendererComponent = g_Engine.m_MeshRenderSystem->GetComponent(m_EntityId);
    }
}
