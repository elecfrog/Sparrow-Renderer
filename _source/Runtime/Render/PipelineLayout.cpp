#include "PipelineLayout.h"
#include <stdexcept>

namespace Sparrow
{
    PipelineLayout::~PipelineLayout()
    {
        Cleanup();
    }

    void PipelineLayout::AddUBO(const std::string& name, size_t size, bool isDynamic)
    {
        if (m_Initialized)
        {
            throw std::runtime_error("Cannot add UBO after PipelineLayout initialization");
        }

        UBODescriptor descriptor;
        descriptor.name = name;
        descriptor.bindingPoint = static_cast<GLuint>(m_UBOs.size()); // 自动分配绑定点
        descriptor.size = size;
        descriptor.isDynamic = isDynamic;

        UBOInfo info;
        info.descriptor = descriptor;
        info.buffer = 0;

        m_UBOs[name] = info;
    }

    void PipelineLayout::Init()
    {
        if (m_Initialized) return;

        for (auto& [name, info] : m_UBOs)
        {
            // 创建缓冲区
            glGenBuffers(1, &info.buffer);
            glBindBuffer(GL_UNIFORM_BUFFER, info.buffer);
            
            // 分配内存
            GLenum usage = info.descriptor.isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
            glBufferData(GL_UNIFORM_BUFFER, info.descriptor.size, nullptr, usage);
            
            // 绑定到对应的绑定点
            glBindBufferBase(GL_UNIFORM_BUFFER, info.descriptor.bindingPoint, info.buffer);
        }

        m_Initialized = true;
    }

    void PipelineLayout::Cleanup()
    {
        if (!m_Initialized) return;

        for (auto& [name, info] : m_UBOs)
        {
            if (info.buffer != 0)
            {
                glDeleteBuffers(1, &info.buffer);
                info.buffer = 0;
            }
        }

        m_UBOs.clear();
        m_Initialized = false;
    }

    void PipelineLayout::UpdateUBO(const std::string& name, const void* data)
    {
        auto it = m_UBOs.find(name);
        if (it == m_UBOs.end())
        {
            throw std::runtime_error("UBO not found: " + name);
        }

        glBindBuffer(GL_UNIFORM_BUFFER, it->second.buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, it->second.descriptor.size, data);
    }

    GLuint PipelineLayout::GetUBOBindingPoint(const std::string& name) const
    {
        auto it = m_UBOs.find(name);
        if (it == m_UBOs.end())
        {
            throw std::runtime_error("UBO not found: " + name);
        }
        return it->second.descriptor.bindingPoint;
    }

    GLuint PipelineLayout::GetUBOBuffer(const std::string& name) const
    {
        auto it = m_UBOs.find(name);
        if (it == m_UBOs.end())
        {
            throw std::runtime_error("UBO not found: " + name);
        }
        return it->second.buffer;
    }
} 