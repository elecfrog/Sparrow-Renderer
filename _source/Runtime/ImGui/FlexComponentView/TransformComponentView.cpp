#include "TransformComponentView.h"
#include "imgui.h"
#include "Engine/Engine.h"

namespace Sparrow
{
    void TransformComponentView::OnImGuiRender()
    {
        if (!m_TransformComponent)
        {
            return;
        }

        ImGui::BeginChild("TransformComponent", ImVec2(0, 90));
        if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Position", m_TransformComponent->position.GetPtr(), m_PositionSensitivity);
            // 获取当前欧拉角（临时用于编辑）
            Vector3 euler = m_TransformComponent->rotation.GetEulerAngles();

            bool changed = ImGui::DragFloat3("Rotation", euler.GetPtr(), m_RotationSensitivity);
            // 如果值改变，更新Quaternion
            if (changed)
            {
                m_TransformComponent->rotation.SetFromEulerAngles(euler);
            }
            ImGui::DragFloat3("Scale", m_TransformComponent->scaling.GetPtr(), m_ScaleSensitivity);
        }
        ImGui::EndChild();
    }

}