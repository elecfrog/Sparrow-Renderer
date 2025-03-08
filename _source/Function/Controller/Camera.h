#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "spdlog/spdlog.h"

class Camera {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float FOV;
    float keySensitivity;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    Camera() :
            cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)),
            cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
            cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            yaw(270.0f), pitch(0.0f), FOV(45.0f), keySensitivity(1.0f) {

    }

    explicit Camera(glm::vec3 _pos, float yaw, float pitch, float fov, float ks)
            : cameraPos(_pos), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
              yaw(yaw), pitch(pitch), FOV(fov), keySensitivity(ks) {}


    inline void CameraOnZoom(double x_offset, double y_offset)
    {
        spdlog::info("CameraOnZoom, x_offset{0}, y_offset{1}", x_offset, y_offset);
        cameraPos = glm::vec3(1.f);
        // spdlog::info("pos: {0} {1} {2}", cameraPos.x, cameraPos.y, cameraPos.z);
    }

    void UpdateCameraMatrix();

    const glm::mat4 &GetViewMatrix() const;

    glm::mat4 GetProjMatrix() const;
};