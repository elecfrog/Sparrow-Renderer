#pragma once

#include "UBO.h"
#include "RHI/OpenGL.h"

namespace Sparrow
{
    class UBOManager
    {
    public:
        static void Init();
        static void Cleanup();

        // 更新UBO数据
        static void UpdateMatrices(const MatricesUBO& matrices);
        static void UpdateCamera(const CameraUBO& camera);
        static void UpdateLight(const LightUBO& light);

        // 获取UBO绑定点
        static GLuint GetMatricesBindingPoint() { return 0; }
        static GLuint GetCameraBindingPoint() { return 1; }
        static GLuint GetLightBindingPoint() { return 2; }

    private:
        static GLuint m_MatricesUBO;
        static GLuint m_CameraUBO;
        static GLuint m_LightUBO;
    };
}