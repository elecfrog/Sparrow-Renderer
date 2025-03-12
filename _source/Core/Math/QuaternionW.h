#pragma once

#include "MathUtils.h"

namespace Sparrow
{
    class QuaternionW
    {
    public:
        // 构造函数
        QuaternionW() : m_Value(1.0f, 0.0f, 0.0f, 0.0f) {} // Identity quaternion (w=1, x=y=z=0)
        QuaternionW(float w, float x, float y, float z) : m_Value(w, x, y, z) {}
        QuaternionW(const glm::quat& q) : m_Value(q) {}
        
        // 静态创建方法
        static QuaternionW Identity() { return QuaternionW(1.0f, 0.0f, 0.0f, 0.0f); }
        
        // 从欧拉角创建（角度制）
        static QuaternionW FromEuler(float pitch, float yaw, float roll)
        {
            return QuaternionW(glm::quat(glm::vec3(
                glm::radians(pitch),
                glm::radians(yaw),
                glm::radians(roll)
            )));
        }
        
        static QuaternionW FromEuler(glm::vec3 euler);

        // 从轴角创建
        static QuaternionW FromAxisAngle(const glm::vec3& axis, float angleDegrees)
        {
            return QuaternionW(glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis)));
        }

        // 获取欧拉角（角度制）
        glm::vec3 ToEuler() const
        {
            return glm::degrees(glm::eulerAngles(m_Value));
        }

        // 获取轴角
        void ToAxisAngle(glm::vec3& outAxis, float& outAngleDegrees) const
        {
            float angle;
            glm::vec3 axis;
            angle = 2.0f * std::acos(m_Value.w);
            float s = std::sqrt(1.0f - m_Value.w * m_Value.w);
            if (s < 0.0001f)
            {
                axis.x = m_Value.x;
                axis.y = m_Value.y;
                axis.z = m_Value.z;
            }
            else
            {
                axis.x = m_Value.x / s;
                axis.y = m_Value.y / s;
                axis.z = m_Value.z / s;
            }
            outAxis = axis;
            outAngleDegrees = glm::degrees(angle);
        }

        // 基本运算
        QuaternionW operator*(const QuaternionW& other) const { return QuaternionW(m_Value * other.m_Value); }
        QuaternionW& operator*=(const QuaternionW& other) { m_Value *= other.m_Value; return *this; }
        
        // 向量旋转
        glm::vec3 RotateVector(const glm::vec3& v) const
        {
            return m_Value * v;
        }

        // 插值
        static QuaternionW Slerp(const QuaternionW& a, const QuaternionW& b, float t)
        {
            return QuaternionW(glm::slerp(a.m_Value, b.m_Value, t));
        }

        // 标准化
        void Normalize()
        {
            m_Value = glm::normalize(m_Value);
        }

        QuaternionW Normalized() const
        {
            return QuaternionW(glm::normalize(m_Value));
        }

        // 求逆
        QuaternionW Inverse() const
        {
            return QuaternionW(glm::inverse(m_Value));
        }

        // 点积
        float Dot(const QuaternionW& other) const
        {
            return glm::dot(m_Value, other.m_Value);
        }

        // 获取原始glm::quat
        const glm::quat& GetGLMQuat() const { return m_Value; }
        
        // 获取各个分量
        float GetW() const { return m_Value.w; }
        float GetX() const { return m_Value.x; }
        float GetY() const { return m_Value.y; }
        float GetZ() const { return m_Value.z; }

        // 设置分量
        void Set(float w, float x, float y, float z)
        {
            m_Value.w = w;
            m_Value.x = x;
            m_Value.y = y;
            m_Value.z = z;
        }

        // 欧拉角支持
        Vector3 GetEulerAngles() const
        {
            return Vector3(glm::eulerAngles(m_Value) * glm::degrees(1.0f));
        }

        void SetFromEulerAngles(const Vector3& euler)
        {
            m_Value = glm::quat(glm::radians(euler.GetGLMVec3()));
        }

        // 获取连续内存指针（用于ImGui等）
        const float* GetPtr() const { return &m_Value[0]; }
        float* GetPtr() { return &m_Value[0]; }

        // 通过索引访问元素
        float& operator[](int index) { return (&m_Value.w)[index]; }
        const float& operator[](int index) const { return (&m_Value.w)[index]; }

    private:
        glm::quat m_Value;
    };

    inline QuaternionW QuaternionW::FromEuler(glm::vec3 euler)
    {
        return QuaternionW(glm::quat(euler));
    }
}