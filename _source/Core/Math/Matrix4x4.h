#pragma once


#include <glm/gtx/matrix_decompose.hpp>

#include "MathUtils.h"
#include "Vector3.h"
#include "Vector4.h"
#include "QuaternionW.h"

namespace Sparrow
{
    class Matrix4x4
    {
    public:
        // 构造函数
        Matrix4x4() : m_Value(1.0f) {} // 默认为单位矩阵
        Matrix4x4(const glm::mat4& m) : m_Value(m) {}
        Matrix4x4(float diagonal) : m_Value(diagonal) {}

        // 静态创建方法
        static Matrix4x4 Identity() { return Matrix4x4(1.0f); }

        // 变换矩阵创建
        static Matrix4x4 CreateTranslation(const Vector3& position)
        {
            return Matrix4x4(glm::translate(glm::mat4(1.0f), position.GetGLMVec3()));
        }

        static Matrix4x4 CreateRotation(const QuaternionW& rotation)
        {
            return Matrix4x4(glm::mat4_cast(rotation.GetGLMQuat()));
        }

        static Matrix4x4 CreateScale(const Vector3& scale)
        {
            return Matrix4x4(glm::scale(glm::mat4(1.0f), scale.GetGLMVec3()));
        }

        static Matrix4x4 CreateTRS(const Vector3& translation, const QuaternionW& rotation, const Vector3& scale)
        {
            Matrix4x4 result;
            result.SetTRS(translation, rotation, scale);
            return result;
        }

        // 视图和投影矩阵创建
        static Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
        {
            return Matrix4x4(glm::lookAt(eye.GetGLMVec3(), target.GetGLMVec3(), up.GetGLMVec3()));
        }

        static Matrix4x4 CreatePerspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
        {
            return Matrix4x4(glm::perspective(glm::radians(fovDegrees), aspect, nearPlane, farPlane));
        }

        static Matrix4x4 CreateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
        {
            return Matrix4x4(glm::ortho(left, right, bottom, top, nearPlane, farPlane));
        }

        // 矩阵运算
        Matrix4x4 operator*(const Matrix4x4& other) const { return Matrix4x4(m_Value * other.m_Value); }
        Vector4 operator*(const Vector4& vector) const { return Vector4(m_Value * vector.GetGLMVec4()); }
        Matrix4x4& operator*=(const Matrix4x4& other) { m_Value *= other.m_Value; return *this; }

        // 矩阵变换
        Matrix4x4 Inverse() const { return Matrix4x4(glm::inverse(m_Value)); }
        Matrix4x4 Transpose() const { return Matrix4x4(glm::transpose(m_Value)); }

        // 分解变换
        void Decompose(Vector3& translation, QuaternionW& rotation, Vector3& scale) const
        {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::vec3 t, s;
            glm::quat r;
            glm::decompose(m_Value, s, r, t, skew, perspective);
            
            translation = Vector3(t);
            rotation = QuaternionW(r);
            scale = Vector3(s);
        }

        // 设置TRS
        void SetTRS(const Vector3& translation, const QuaternionW& rotation, const Vector3& scale)
        {
            m_Value = glm::translate(glm::mat4(1.0f), translation.GetGLMVec3()) *
                     glm::mat4_cast(rotation.GetGLMQuat()) *
                     glm::scale(glm::mat4(1.0f), scale.GetGLMVec3());
        }

        // 获取矩阵元素
        float Get(int row, int col) const { return m_Value[col][row]; }
        void Set(int row, int col, float value) { m_Value[col][row] = value; }

        // 获取连续内存指针（用于OpenGL等）
        const float* GetPtr() const { return &m_Value[0][0]; }
        float* GetPtr() { return &m_Value[0][0]; }

        // 通过索引访问元素（用于连续内存访问）
        float& operator[](int index) { return GetPtr()[index]; }
        const float& operator[](int index) const { return GetPtr()[index]; }

        // 获取行和列
        Vector4 GetRow(int row) const
        {
            return Vector4(m_Value[0][row], m_Value[1][row], m_Value[2][row], m_Value[3][row]);
        }

        Vector4 GetColumn(int col) const
        {
            return Vector4(m_Value[col]);
        }

        // 获取特定部分
        Vector3 GetTranslation() const { return Vector3(m_Value[3]); }
        Vector3 GetScale() const
        {
            return Vector3(
                glm::length(Vector3(m_Value[0]).GetGLMVec3()),
                glm::length(Vector3(m_Value[1]).GetGLMVec3()),
                glm::length(Vector3(m_Value[2]).GetGLMVec3())
            );
        }

        // 变换向量
        Vector3 TransformPoint(const Vector3& point) const
        {
            Vector4 temp = (*this) * Vector4(point, 1.0f);
            return temp.ToPoint3();
        }

        Vector3 TransformVector(const Vector3& vector) const
        {
            return ((*this) * Vector4(vector, 0.0f)).ToVector3();
        }

        Vector3 TransformDirection(const Vector3& direction) const
        {
            Vector3 result = TransformVector(direction);
            result.Normalize();
            return result;
        }

        // 获取原始glm::mat4
        const glm::mat4& GetGLMMat4() const { return m_Value; }
        glm::mat4& GetGLMMat4() { return m_Value; }

        // 类型转换
        operator glm::mat4() const { return m_Value; }

    private:
        glm::mat4 m_Value;
    };
} 