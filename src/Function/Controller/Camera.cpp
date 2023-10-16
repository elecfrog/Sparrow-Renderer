#include "Camera.h"

const glm::mat4 &Camera::GetViewMatrix() const {
    return this->viewMatrix;
}

glm::mat4 Camera::GetProjMatrix() const {
    return this->projMatrix;
}

void Camera::UpdateCameraMatrix() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) *
                  cos(glm::radians(this->pitch)); // Note that we convert the angle to radians first
    direction.y = sin(glm::radians(this->pitch)); // Note that we convert the angle to radians first
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->cameraFront = glm::normalize(direction);

    this->viewMatrix = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
    );

    this->projMatrix = glm::perspective(
            glm::radians(this->FOV),
            16.0f / 9.0f,
            0.1f, 100.0f);
}

