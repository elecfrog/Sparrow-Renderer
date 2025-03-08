#pragma once

#include <vector>
#include <glad/glad.h>

class IndexBuffer {
private:
    unsigned int m_rendererId;
    unsigned int m_count;

public:
    IndexBuffer() = default;

    IndexBuffer(const unsigned int *data, unsigned int count);

    IndexBuffer(std::vector<GLuint> &indices);

    ~IndexBuffer();

    // Binds the IndexBuffer
    void Bind() const;

    // Unbinds the IndexBuffer
    void Unbind() const;

    unsigned int GetCount() const { return m_count; }

protected:
};

using IBO = IndexBuffer;
using EBO = IndexBuffer;