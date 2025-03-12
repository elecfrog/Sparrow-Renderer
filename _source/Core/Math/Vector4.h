#pragma once

#include "Vector3.h"
#include <cereal/cereal.hpp>

namespace Sparrow
{
    class Vector4
    {
    public:
        // 构造函数
        Vector4() : m_Value(0.0f)
        {
        }

        Vector4(float x, float y, float z, float w) : m_Value(x, y, z, w)
        {
        }

        Vector4(float value) : m_Value(value)
        {
        }

        Vector4(const glm::vec4& v) : m_Value(v)
        {
        }

        Vector4(const Vector3& v, float w) : m_Value(v.GetX(), v.GetY(), v.GetZ(), w)
        {
        }

        // 常用静态向量
        static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
        static Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }

        // 基本运算符
        Vector4 operator+(const Vector4& other) const { return Vector4(m_Value + other.m_Value); }
        Vector4 operator-(const Vector4& other) const { return Vector4(m_Value - other.m_Value); }
        Vector4 operator*(float scalar) const { return Vector4(m_Value * scalar); }
        Vector4 operator/(float scalar) const { return Vector4(m_Value / scalar); }
        Vector4 operator-() const { return Vector4(-m_Value); }

        // 复合赋值运算符
        Vector4& operator+=(const Vector4& other)
        {
            m_Value += other.m_Value;
            return *this;
        }

        Vector4& operator-=(const Vector4& other)
        {
            m_Value -= other.m_Value;
            return *this;
        }

        Vector4& operator*=(float scalar)
        {
            m_Value *= scalar;
            return *this;
        }

        Vector4& operator/=(float scalar)
        {
            m_Value /= scalar;
            return *this;
        }

        // 比较运算符
        bool operator==(const Vector4& other) const { return m_Value == other.m_Value; }
        bool operator!=(const Vector4& other) const { return m_Value != other.m_Value; }

        // 向量运算
        float Dot(const Vector4& other) const { return glm::dot(m_Value, other.m_Value); }
        float Length() const { return glm::length(m_Value); }
        float LengthSquared() const { return glm::length2(m_Value); }
        float Distance(const Vector4& other) const { return glm::distance(m_Value, other.m_Value); }
        float DistanceSquared(const Vector4& other) const { return glm::distance2(m_Value, other.m_Value); }

        // 标准化
        void Normalize() { m_Value = glm::normalize(m_Value); }
        Vector4 Normalized() const { return Vector4(glm::normalize(m_Value)); }

        // 分量操作
        float GetX() const { return m_Value.x; }
        float GetY() const { return m_Value.y; }
        float GetZ() const { return m_Value.z; }
        float GetW() const { return m_Value.w; }
        void SetX(float x) { m_Value.x = x; }
        void SetY(float y) { m_Value.y = y; }
        void SetZ(float z) { m_Value.z = z; }
        void SetW(float w) { m_Value.w = w; }

        // 设置所有分量
        void Set(float x, float y, float z, float w)
        {
            m_Value.x = x;
            m_Value.y = y;
            m_Value.z = z;
            m_Value.w = w;
        }

        // 转换为Vector3
        Vector3 ToVector3() const { return Vector3(m_Value.x, m_Value.y, m_Value.z); }

        // 获取齐次坐标下的3D点
        Vector3 ToPoint3() const
        {
            if (m_Value.w != 0.0f)
                return Vector3(m_Value.x / m_Value.w, m_Value.y / m_Value.w, m_Value.z / m_Value.w);
            return Vector3(m_Value.x, m_Value.y, m_Value.z);
        }

        // 获取最小/最大分量
        float Min() const { return glm::min(glm::min(glm::min(m_Value.x, m_Value.y), m_Value.z), m_Value.w); }
        float Max() const { return glm::max(glm::max(glm::max(m_Value.x, m_Value.y), m_Value.z), m_Value.w); }

        // 分量间运算
        Vector4 ComponentMin(const Vector4& other) const { return Vector4(glm::min(m_Value, other.m_Value)); }
        Vector4 ComponentMax(const Vector4& other) const { return Vector4(glm::max(m_Value, other.m_Value)); }

        // 获取原始glm::vec4
        const glm::vec4& GetGLMVec4() const { return m_Value; }
        glm::vec4& GetGLMVec4() { return m_Value; }

        // 类型转换
        operator glm::vec4() const { return m_Value; }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(
                cereal::make_nvp("vector4", m_Value)
            );
        }

    private:
        glm::vec4 m_Value;
    };

    // 标量乘法（左乘）
    inline Vector4 operator*(float scalar, const Vector4& vector)
    {
        return vector * scalar;
    }
}
