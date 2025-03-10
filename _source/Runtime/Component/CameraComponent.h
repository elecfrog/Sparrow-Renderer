#pragma once

#include <Core/Core.h>

namespace Sparrow
{
    struct CameraComponent
    {
        Vector3f cameraPos;
        Vector3f cameraFront;
        Vector3f cameraUp;

        float yaw;
        float pitch;
        float FOV;
        float keySensitivity;

        Matrix4f viewMatrix;
        Matrix4f projMatrix;

        CameraComponent() :
            cameraPos(Vector3f(0.0f, 0.0f, 3.0f)),
            cameraFront(Vector3f(0.0f, 0.0f, -1.0f)),
            cameraUp(Vector3f(0.0f, 1.0f, 0.0f)),
            yaw(270.0f), pitch(0.0f), FOV(45.0f), keySensitivity(1.0f)
        {
        }

        explicit CameraComponent(const Vector3f& _pos, float yaw, float pitch, float fov, float ks)
            : cameraPos(_pos), cameraFront(Vector3f(0.0f, 0.0f, -1.0f)), cameraUp(Vector3f(0.0f, 1.0f, 0.0f)),
              yaw(yaw), pitch(pitch), FOV(fov), keySensitivity(ks)
        {
        }

        void CameraOnZoom(double x_offset, double y_offset)
        {
            LOG_INFO(LogModule::Render, "CameraOnZoom, x_offset{}, y_offset{}", x_offset, y_offset);
            cameraPos = Vector3f(1.f);
        }

        void UpdateCameraMatrix();

        const Matrix4f& GetViewMatrix() const;

        Matrix4f GetProjMatrix() const;
    };
}
