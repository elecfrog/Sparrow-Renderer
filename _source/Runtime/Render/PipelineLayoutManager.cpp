#include "PipelineLayoutManager.h"

namespace Sparrow
{
    SharedPtr<PipelineLayout> PipelineLayoutManager::CreatePipelineLayout(const std::string& name)
    {
        if (m_PipelineLayouts.find(name) != m_PipelineLayouts.end())
        {
            throw std::runtime_error("PipelineLayout already exists: " + name);
        }

        auto layout = std::make_shared<PipelineLayout>();
        m_PipelineLayouts[name] = layout;
        return layout;
    }

    SharedPtr<PipelineLayout> PipelineLayoutManager::GetPipelineLayout(const std::string& name)
    {
        auto it = m_PipelineLayouts.find(name);
        if (it == m_PipelineLayouts.end())
        {
            throw std::runtime_error("PipelineLayout not found: " + name);
        }
        return it->second;
    }

    void PipelineLayoutManager::DeletePipelineLayout(const std::string& name)
    {
        auto it = m_PipelineLayouts.find(name);
        if (it != m_PipelineLayouts.end())
        {
            m_PipelineLayouts.erase(it);
        }
    }

    void PipelineLayoutManager::InitAll()
    {
        for (auto& [name, layout] : m_PipelineLayouts)
        {
            layout->Init();
        }
    }

    void PipelineLayoutManager::CleanupAll()
    {
        for (auto& [name, layout] : m_PipelineLayouts)
        {
            layout->Cleanup();
        }
        m_PipelineLayouts.clear();
    }
} 