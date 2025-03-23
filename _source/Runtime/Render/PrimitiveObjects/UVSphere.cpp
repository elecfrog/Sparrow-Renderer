/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "UVSphere.h"
#include "Engine/Engine.h"
#include "System/MeshSystem.h"
#include "System/MeshRenderSystem.h"

namespace Sparrow
{
    UVSphere::UVSphere(Vector3 center, Float radius)
        : m_Center(center), m_Radius(radius)
    {
        BuildMeshComponent();
        BuildMeshRendererComponent();
    }

    void UVSphere::BuildMeshComponent()
    {
        const UInt X_SEGMENTS = 64;
        const UInt Y_SEGMENTS = 64;
        const Float PI = 3.14159265359f;

        MeshComponent component;

        // 计算顶点数量
        UInt vertexCount = (X_SEGMENTS + 1) * (Y_SEGMENTS + 1);
        UInt indexCount = X_SEGMENTS * Y_SEGMENTS * 2; // 每个四边形由两个三角形组成

        // 初始化缓冲区
        component.m_PositionBuffer.resize(vertexCount);
        component.m_NormalsBuffer.resize(vertexCount);
        component.m_TangentsBuffer.resize(vertexCount);
        component.m_TexCoord0Buffer.resize(vertexCount);
        component.m_IndicesBuffer.resize(indexCount * 3); // 每个三角形3个顶点

        // 生成顶点
        for (UInt x = 0; x <= X_SEGMENTS; ++x)
        {
            for (UInt y = 0; y <= Y_SEGMENTS; ++y)
            {
                Float xSegment = static_cast<Float>(x) / static_cast<Float>(X_SEGMENTS);
                Float ySegment = static_cast<Float>(y) / static_cast<Float>(Y_SEGMENTS);
                Float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                Float yPos = std::cos(ySegment * PI);
                Float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                UInt index = y * (X_SEGMENTS + 1) + x;
                component.m_PositionBuffer[index] = Vector3(xPos, yPos, zPos) * m_Radius + m_Center;
                component.m_NormalsBuffer[index] = Vector3(xPos, yPos, zPos);
                component.m_TangentsBuffer[index] = Vector3(0.0f);
                component.m_TexCoord0Buffer[index] = Vector2(xSegment, ySegment);
            }
        }

        // 生成索引
        UInt indexOffset = 0;
        bool oddRow = false;
        for (UInt y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // 偶数行
            {
                for (UInt x = 0; x <= X_SEGMENTS; ++x)
                {
                    component.m_IndicesBuffer[indexOffset++] = y * (X_SEGMENTS + 1) + x;
                    component.m_IndicesBuffer[indexOffset++] = (y + 1) * (X_SEGMENTS + 1) + x;
                }
            }
            else // 奇数行
            {
                for (Int x = X_SEGMENTS; x >= 0; --x)
                {
                    component.m_IndicesBuffer[indexOffset++] = (y + 1) * (X_SEGMENTS + 1) + x;
                    component.m_IndicesBuffer[indexOffset++] = y * (X_SEGMENTS + 1) + x;
                }
            }
            oddRow = !oddRow;
        }

        // 设置顶点和索引计数
        component.m_VertexCount = vertexCount;
        component.m_IndexCount = indexOffset;

        g_Engine.m_MeshSystem->AddComponent(m_EntityId, std::move(component));
        m_MeshComponent = g_Engine.m_MeshSystem->GetComponent(m_EntityId);
    }

    void UVSphere::BuildMeshRendererComponent()
    {
        MeshRendererComponent renderer_component(*m_MeshComponent);
        g_Engine.m_MeshRenderSystem->AddComponent(m_EntityId, std::move(renderer_component));
        m_MeshRendererComponent = g_Engine.m_MeshRenderSystem->GetComponent(m_EntityId);
        m_MeshRendererComponent->m_RenderMode = RenderMode::PerTriangle_Strip;
    }
}
