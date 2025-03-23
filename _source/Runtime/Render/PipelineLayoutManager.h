#pragma once

#include "PipelineLayout.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Sparrow
{
    class PipelineLayoutManager
    {
    public:
        PipelineLayoutManager() = default;
        ~PipelineLayoutManager() = default;
        PipelineLayoutManager(const PipelineLayoutManager&) = delete;
        PipelineLayoutManager& operator=(const PipelineLayoutManager&) = delete;

        // 创建新的PipelineLayout
        SharedPtr<PipelineLayout> CreatePipelineLayout(const std::string& name);
        
        // 获取PipelineLayout
        SharedPtr<PipelineLayout> GetPipelineLayout(const std::string& name);
        
        // 删除PipelineLayout
        void DeletePipelineLayout(const std::string& name);

        // 初始化所有PipelineLayout
        void InitAll();
        
        // 清理所有PipelineLayout
        void CleanupAll();

    private:

        std::unordered_map<std::string, SharedPtr<PipelineLayout>> m_PipelineLayouts;
    };
} 