/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Cylinder.h"
#include "Render/MeshRenderer.h"
#include "Engine/Engine.h"
#include "System/MeshSystem.h"
#include "System/MeshRenderSystem.h"

namespace Sparrow
{
    Cylinder::Cylinder(Vector3f start, Vector3f end)
        : start(start), end(end), m_MeshComponent(nullptr), m_MeshRendererComponent(nullptr)
    {
    }

    void Cylinder::BuildMeshComponent()
    {
        UInt  N = 30;
        Float R = 1.3f;

        MeshComponent component;

        // 计算顶点数量
        UInt vertexCount = 4 * N + 2;  // 4N个侧面顶点 + 2个中心顶点
        UInt indexCount = 12 * N;      // 6N个侧面三角形 + 3N个顶面三角形 + 3N个底面三角形

        // 初始化缓冲区
        component.m_PositionBuffer.resize(vertexCount);
        component.m_NormalsBuffer.resize(vertexCount);
        component.m_TangentsBuffer.resize(vertexCount);
        component.m_TexCoord0Buffer.resize(vertexCount);
        component.m_IndicesBuffer.resize(indexCount);

        // 设置中心顶点
        component.m_PositionBuffer[4 * N + 0] = start;     // 顶面中心
        component.m_PositionBuffer[4 * N + 1] = end;       // 底面中心

        // 计算法线
        Vector3f top_normal = glm::normalize(start - end);
        Vector3f bottom_normal = -top_normal;
        component.m_NormalsBuffer[4 * N + 0] = top_normal;    // 顶面中心法线
        component.m_NormalsBuffer[4 * N + 1] = bottom_normal; // 底面中心法线

        // 计算圆柱体的基向量
        Vector3f normal = glm::normalize(end - start);
        Vector3f tangent = Maths::ComputeOrthogonalVector(normal);
        Vector3f biTangent = glm::normalize(glm::cross(normal, tangent));

        // 生成侧面顶点
        for (UInt i = 0; i < N; i++)
        {
            Float angle = i * 2 * Maths::M_PI / N;
            Float nx = cos(angle);
            Float ny = sin(angle);
            Vector3f offset = nx * tangent + ny * biTangent;

            // 设置顶点位置
            component.m_PositionBuffer[2 * i + 0] = start + R * offset;     // 顶面边缘
            component.m_PositionBuffer[2 * i + 1] = end + R * offset;       // 底面边缘
            component.m_PositionBuffer[2 * N + i] = start + R * offset;     // 顶面边缘（重复）
            component.m_PositionBuffer[3 * N + i] = end + R * offset;       // 底面边缘（重复）

            // 设置法线
            component.m_NormalsBuffer[2 * i + 0] = offset;     // 侧面法线
            component.m_NormalsBuffer[2 * i + 1] = offset;     // 侧面法线
            component.m_NormalsBuffer[2 * N + i] = top_normal;    // 顶面法线
            component.m_NormalsBuffer[3 * N + i] = bottom_normal; // 底面法线

            // 设置切线（这里使用零向量，因为圆柱体不需要切线）
            component.m_TangentsBuffer[2 * i + 0] = Vector3f(0.0f);
            component.m_TangentsBuffer[2 * i + 1] = Vector3f(0.0f);
            component.m_TangentsBuffer[2 * N + i] = Vector3f(0.0f);
            component.m_TangentsBuffer[3 * N + i] = Vector3f(0.0f);

            // 设置纹理坐标
            Float u = static_cast<Float>(i) / N;
            component.m_TexCoord0Buffer[2 * i + 0] = Vector2f(u, 0.0f);
            component.m_TexCoord0Buffer[2 * i + 1] = Vector2f(u, 1.0f);
            component.m_TexCoord0Buffer[2 * N + i] = Vector2f(u, 0.0f);
            component.m_TexCoord0Buffer[3 * N + i] = Vector2f(u, 1.0f);

            // 设置索引
            UInt j = (i + 1) % N;

            // 侧面三角形
            component.m_IndicesBuffer[6 * i + 0] = 2 * j + 0;     // 顶面边缘
            component.m_IndicesBuffer[6 * i + 1] = 2 * j + 1;     // 底面边缘
            component.m_IndicesBuffer[6 * i + 2] = 2 * i + 0;     // 当前顶面边缘
            component.m_IndicesBuffer[6 * i + 3] = 2 * j + 1;     // 底面边缘
            component.m_IndicesBuffer[6 * i + 4] = 2 * i + 1;     // 当前底面边缘
            component.m_IndicesBuffer[6 * i + 5] = 2 * i + 0;     // 当前顶面边缘

            // 顶面三角形
            component.m_IndicesBuffer[6 * N + 3 * i + 0] = 2 * N + i;
            component.m_IndicesBuffer[6 * N + 3 * i + 1] = 2 * N + j;
            component.m_IndicesBuffer[6 * N + 3 * i + 2] = 4 * N + 0;  // 顶面中心

            // 底面三角形
            component.m_IndicesBuffer[9 * N + 3 * i + 0] = 3 * N + i;
            component.m_IndicesBuffer[9 * N + 3 * i + 1] = 3 * N + j;
            component.m_IndicesBuffer[9 * N + 3 * i + 2] = 4 * N + 1;  // 底面中心
        }

        // 设置中心顶点的切线
        component.m_TangentsBuffer[4 * N + 0] = Vector3f(0.0f);
        component.m_TangentsBuffer[4 * N + 1] = Vector3f(0.0f);

        // 设置中心顶点的纹理坐标
        component.m_TexCoord0Buffer[4 * N + 0] = Vector2f(0.5f, 0.5f);
        component.m_TexCoord0Buffer[4 * N + 1] = Vector2f(0.5f, 0.5f);

        // 设置顶点和索引计数
        component.m_VertexCount = vertexCount;
        component.m_IndexCount = indexCount;

        g_Engine.m_MeshSystem->AddComponent(m_EntityId, std::move(component));
        m_MeshComponent = g_Engine.m_MeshSystem->GetComponent(m_EntityId);
    }

    void Cylinder::BuildMeshRendererComponent(SharedPtr<Shader> shader)
    {
        MeshRendererComponent renderer_component(*m_MeshComponent, shader);
        g_Engine.m_MeshRenderSystem->AddComponent(m_EntityId, std::move(renderer_component));
        m_MeshRendererComponent = g_Engine.m_MeshRenderSystem->GetComponent(m_EntityId);
        m_MeshRendererComponent->m_RenderMode = RenderMode::PerTriangle;
    }
}
