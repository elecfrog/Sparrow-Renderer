#pragma once

#include "Component/TransformComponent.h"
#include "Entity/Entity.h"

namespace Sparrow
{
    class TransformComponentView
    {
    public:
        TransformComponentView()  = default;
        ~TransformComponentView() = default;

        void OnImGuiRender();

        // 设置要编辑的Transform数据
        void SetTransformComponent(TransformComponent* transformComponent) { m_TransformComponent = transformComponent; }

        // 设置编辑精度
        void SetPositionSensitivity(float sensitivity) { m_PositionSensitivity = sensitivity; }
        void SetRotationSensitivity(float sensitivity) { m_RotationSensitivity = sensitivity; }
        void SetScaleSensitivity(float sensitivity) { m_ScaleSensitivity = sensitivity; }

    private:
        TransformComponent* m_TransformComponent = nullptr;

        // 编辑精度
        float m_PositionSensitivity = 0.01f;
        float m_RotationSensitivity = 0.1f;
        float m_ScaleSensitivity = 0.01f;
    };
}
