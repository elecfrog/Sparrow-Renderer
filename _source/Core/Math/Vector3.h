#pragma once


#include "Maths.h"

namespace Sparrow
{
    class Vector3
    {
    public:
        // 构造函数
        Vector3() : m_Value(0.0f) {}
        Vector3(float x, float y, float z) : m_Value(x, y, z) {}
        Vector3(float value) : m_Value(value) {}
        Vector3(const glm::vec3& v) : m_Value(v) {}

        // 常用静态向量
        static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
        static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
        static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
        static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
        static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
        static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
        static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }
        static Vector3 Back() { return Vector3(0.0f, 0.0f, -1.0f); }

        // 基本运算符
        Vector3 operator+(const Vector3& other) const { return Vector3(m_Value + other.m_Value); }
        Vector3 operator-(const Vector3& other) const { return Vector3(m_Value - other.m_Value); }
        Vector3 operator*(float scalar) const { return Vector3(m_Value * scalar); }
        Vector3 operator/(float scalar) const { return Vector3(m_Value / scalar); }
        Vector3 operator-() const { return Vector3(-m_Value); }

        // 复合赋值运算符
        Vector3& operator+=(const Vector3& other) { m_Value += other.m_Value; return *this; }
        Vector3& operator-=(const Vector3& other) { m_Value -= other.m_Value; return *this; }
        Vector3& operator*=(float scalar) { m_Value *= scalar; return *this; }
        Vector3& operator/=(float scalar) { m_Value /= scalar; return *this; }

        // 比较运算符
        bool operator==(const Vector3& other) const { return m_Value == other.m_Value; }
        bool operator!=(const Vector3& other) const { return m_Value != other.m_Value; }

        // 向量运算
        float Dot(const Vector3& other) const { return glm::dot(m_Value, other.m_Value); }
        Vector3 Cross(const Vector3& other) const { return Vector3(glm::cross(m_Value, other.m_Value)); }
        float Length() const { return glm::length(m_Value); }
        float LengthSquared() const { return glm::length2(m_Value); }
        float Distance(const Vector3& other) const { return glm::distance(m_Value, other.m_Value); }
        float DistanceSquared(const Vector3& other) const { return glm::distance2(m_Value, other.m_Value); }

        // 标准化
        void Normalize() { m_Value = glm::normalize(m_Value); }
        Vector3 Normalized() const { return Vector3(glm::normalize(m_Value)); }

        // 获取角度（返回角度制）
        float Angle(const Vector3& other) const
        {
            float dot = Dot(other);
            float lengths = Length() * other.Length();
            if (lengths == 0.0f) return 0.0f;
            float angle = std::acos(glm::clamp(dot / lengths, -1.0f, 1.0f));
            return glm::degrees(angle);
        }

        // 反射
        Vector3 Reflect(const Vector3& normal) const
        {
            return Vector3(glm::reflect(m_Value, normal.m_Value));
        }

        // 分量操作
        float GetX() const { return m_Value.x; }
        float GetY() const { return m_Value.y; }
        float GetZ() const { return m_Value.z; }
        void SetX(float x) { m_Value.x = x; }
        void SetY(float y) { m_Value.y = y; }
        void SetZ(float z) { m_Value.z = z; }

        // 设置所有分量
        void Set(float x, float y, float z)
        {
            m_Value.x = x;
            m_Value.y = y;
            m_Value.z = z;
        }

        // 获取最小/最大分量
        float Min() const { return glm::min(glm::min(m_Value.x, m_Value.y), m_Value.z); }
        float Max() const { return glm::max(glm::max(m_Value.x, m_Value.y), m_Value.z); }

        // 分量间运算
        Vector3 ComponentMin(const Vector3& other) const { return Vector3(glm::min(m_Value, other.m_Value)); }
        Vector3 ComponentMax(const Vector3& other) const { return Vector3(glm::max(m_Value, other.m_Value)); }

        // 获取原始glm::vec3
        const glm::vec3& GetGLMVec3() const { return m_Value; }
        glm::vec3& GetGLMVec3() { return m_Value; }

        // 获取连续内存指针（用于ImGui等）
        const float* GetPtr() const { return &m_Value[0]; }
        float* GetPtr() { return &m_Value[0]; }

        // 通过索引访问元素
        float& operator[](int index) { return m_Value[index]; }
        const float& operator[](int index) const { return m_Value[index]; }

        // 类型转换
        operator glm::vec3() const { return m_Value; }

    private:
        glm::vec3 m_Value;
    };

    // 标量乘法（左乘）
    inline Vector3 operator*(float scalar, const Vector3& vector)
    {
        return vector * scalar;
    }


} 