#include "IndexBuffer.h"
#include "Core/Utils/Utility.hpp"

IndexBuffer::IndexBuffer(const unsigned *data, unsigned count)
        : m_count(count) {
    ASSERT(sizeof(m_count) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(std::vector<GLuint> &indices)
        : m_count((unsigned int)indices.size()) {
    ASSERT(sizeof(m_count) == sizeof(GLuint));

    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
