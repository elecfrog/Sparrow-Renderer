#include "CameraComponent.h"
#include "Base/BaseDefinition.h"

namespace Sparrow
{
    const Matrix4f& CameraComponent::GetViewMatrix() const
    {
        return viewMatrix;
    }

    Matrix4f CameraComponent::GetProjMatrix() const
    {
        return projMatrix;
    }

    void CameraComponent::UpdateCameraMatrix()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) *
            cos(glm::radians(pitch)); // Note that we convert the angle to radians first
        direction.y = sin(glm::radians(pitch)); // Note that we convert the angle to radians first
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFront = glm::normalize(direction);

        viewMatrix = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        projMatrix = glm::perspective(
            glm::radians(FOV),
            16.0f / 9.0f,
            0.1f, 100.0f);
    }
}
