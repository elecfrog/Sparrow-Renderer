#pragma once

#include <vector>
#include <glad/glad.h>

namespace Sparrow
{
    class IndexBuffer
    {
    private:
        unsigned int m_rendererId;
        GLuint m_count;

    public:
        IndexBuffer() = default;

        IndexBuffer(const unsigned int* data, unsigned int count);

        IndexBuffer(std::vector<GLuint>& indices);

        ~IndexBuffer();

        // Binds the IndexBuffer
        void Bind() const;

        // Unbinds the IndexBuffer
        void Unbind() const;

        GLuint GetCount() const { return m_count; }

    protected:
    };

    using IBO = IndexBuffer;
    using EBO = IndexBuffer;
}
