#pragma once

#include "MathUtils.h"


namespace Sparrow
{
    class Vector2
    {
    public:
        // 构造函数
        Vector2() : m_Value(0.0f)
        {
        }

        Vector2(float x, float y) : m_Value(x, y)
        {
        }

        Vector2(float value) : m_Value(value)
        {
        }

        Vector2(const glm::vec2& v) : m_Value(v)
        {
        }

        // 常用静态向量
        static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
        static Vector2 One() { return Vector2(1.0f, 1.0f); }
        static Vector2 Up() { return Vector2(0.0f, 1.0f); }
        static Vector2 Down() { return Vector2(0.0f, -1.0f); }
        static Vector2 Right() { return Vector2(1.0f, 0.0f); }
        static Vector2 Left() { return Vector2(-1.0f, 0.0f); }

        // 基本运算符
        Vector2 operator+(const Vector2& other) const { return Vector2(m_Value + other.m_Value); }
        Vector2 operator-(const Vector2& other) const { return Vector2(m_Value - other.m_Value); }
        Vector2 operator*(float scalar) const { return Vector2(m_Value * scalar); }
        Vector2 operator/(float scalar) const { return Vector2(m_Value / scalar); }
        Vector2 operator-() const { return Vector2(-m_Value); }

        // 复合赋值运算符
        Vector2& operator+=(const Vector2& other)
        {
            m_Value += other.m_Value;
            return *this;
        }

        Vector2& operator-=(const Vector2& other)
        {
            m_Value -= other.m_Value;
            return *this;
        }

        Vector2& operator*=(float scalar)
        {
            m_Value *= scalar;
            return *this;
        }

        Vector2& operator/=(float scalar)
        {
            m_Value /= scalar;
            return *this;
        }

        // 比较运算符
        bool operator==(const Vector2& other) const { return m_Value == other.m_Value; }
        bool operator!=(const Vector2& other) const { return m_Value != other.m_Value; }

        // 向量运算
        float Dot(const Vector2& other) const { return glm::dot(m_Value, other.m_Value); }
        float Cross(const Vector2& other) const { return m_Value.x * other.m_Value.y - m_Value.y * other.m_Value.x; }
        float Length() const { return glm::length(m_Value); }
        float LengthSquared() const { return glm::length2(m_Value); }
        float Distance(const Vector2& other) const { return glm::distance(m_Value, other.m_Value); }
        float DistanceSquared(const Vector2& other) const { return glm::distance2(m_Value, other.m_Value); }

        // 标准化
        void Normalize() { m_Value = glm::normalize(m_Value); }
        Vector2 Normalized() const { return Vector2(glm::normalize(m_Value)); }

        // 获取角度（返回角度制）
        float Angle(const Vector2& other) const
        {
            float dot = Dot(other);
            float lengths = Length() * other.Length();
            if (lengths == 0.0f) return 0.0f;
            float angle = std::acos(glm::clamp(dot / lengths, -1.0f, 1.0f));
            return glm::degrees(angle);
        }

        // 获取方向角（相对于正右方向，返回角度制）
        float DirectionAngle() const
        {
            return glm::degrees(std::atan2(m_Value.y, m_Value.x));
        }

        // 旋转（角度制）
        Vector2 Rotated(float degrees) const
        {
            float rad = glm::radians(degrees);
            float cos = std::cos(rad);
            float sin = std::sin(rad);
            return Vector2(
                m_Value.x * cos - m_Value.y * sin,
                m_Value.x * sin + m_Value.y * cos
            );
        }

        void Rotate(float degrees)
        {
            *this = Rotated(degrees);
        }

        // 分量操作
        float GetX() const { return m_Value.x; }
        float GetY() const { return m_Value.y; }
        void SetX(float x) { m_Value.x = x; }
        void SetY(float y) { m_Value.y = y; }

        // 设置所有分量
        void Set(float x, float y)
        {
            m_Value.x = x;
            m_Value.y = y;
        }

        // 获取最小/最大分量
        float Min() const { return glm::min(m_Value.x, m_Value.y); }
        float Max() const { return glm::max(m_Value.x, m_Value.y); }

        // 分量间运算
        Vector2 ComponentMin(const Vector2& other) const { return Vector2(glm::min(m_Value, other.m_Value)); }
        Vector2 ComponentMax(const Vector2& other) const { return Vector2(glm::max(m_Value, other.m_Value)); }

        // 获取原始glm::vec2
        const glm::vec2& GetGLMVec2() const { return m_Value; }
        glm::vec2& GetGLMVec2() { return m_Value; }

        // 类型转换
        operator glm::vec2() const { return m_Value; }

    private:
        glm::vec2 m_Value;
    };

    // 标量乘法（左乘）
    inline Vector2 operator*(float scalar, const Vector2& vector)
    {
        return vector * scalar;
    }
}
