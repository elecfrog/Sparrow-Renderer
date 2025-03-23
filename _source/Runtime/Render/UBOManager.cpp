#include "UBOManager.h"

namespace Sparrow
{
    GLuint UBOManager::m_MatricesUBO = 0;
    GLuint UBOManager::m_CameraUBO = 0;
    GLuint UBOManager::m_LightUBO = 0;

    void UBOManager::Init()
    {
        // 创建并初始化Matrices UBO
        glGenBuffers(1, &m_MatricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(MatricesUBO), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, GetMatricesBindingPoint(), m_MatricesUBO);

        // 创建并初始化Camera UBO
        glGenBuffers(1, &m_CameraUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraUBO), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, GetCameraBindingPoint(), m_CameraUBO);

        // 创建并初始化Light UBO
        glGenBuffers(1, &m_LightUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_LightUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, GetLightBindingPoint(), m_LightUBO);
    }

    void UBOManager::Cleanup()
    {
        if (m_MatricesUBO != 0)
        {
            glDeleteBuffers(1, &m_MatricesUBO);
            m_MatricesUBO = 0;
        }
        if (m_CameraUBO != 0)
        {
            glDeleteBuffers(1, &m_CameraUBO);
            m_CameraUBO = 0;
        }
        if (m_LightUBO != 0)
        {
            glDeleteBuffers(1, &m_LightUBO);
            m_LightUBO = 0;
        }
    }

    void UBOManager::UpdateMatrices(const MatricesUBO& matrices)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_MatricesUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MatricesUBO), &matrices);
    }

    void UBOManager::UpdateCamera(const CameraUBO& camera)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraUBO), &camera);
    }

    void UBOManager::UpdateLight(const LightUBO& light)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_LightUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUBO), &light);
    }
} 