#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

namespace Sparrow
{
    // 对齐到16字节边界
    struct alignas(16) MatricesUBO
    {
        Matrix4x4 model;      // 模型矩阵
        Matrix4x4 view;       // 视图矩阵
        Matrix4x4 projection; // 投影矩阵
    };

    // 对齐到16字节边界
    struct alignas(16) CameraUBO
    {
        Vector3 cameraPos;    // 相机位置
        float padding;        // 填充以保持16字节对齐
    };

    // 对齐到16字节边界
    struct alignas(16) LightUBO
    {
        Vector3 position;     // 光源位置
        float padding1;       // 填充以保持16字节对齐
        Vector3 color;        // 光源颜色
        float padding2;       // 填充以保持16字节对齐
        Vector3 ambient;      // 环境光颜色
        float padding3;       // 填充以保持16字节对齐
    };
} 