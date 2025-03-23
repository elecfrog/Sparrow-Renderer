#pragma once

#include "RHI/OpenGL.h"

namespace Sparrow
{
    struct UBODescriptor
    {
        std::string name;           // UBO名称
        GLuint bindingPoint;        // 绑定点
        size_t size;                // UBO大小
        bool isDynamic;             // 是否是动态UBO
    };

    class PipelineLayout
    {
    public:
        PipelineLayout() = default;
        ~PipelineLayout();

        // 添加UBO描述符
        void AddUBO(const std::string& name, size_t size, bool isDynamic = true);
        
        // 初始化PipelineLayout
        void Init();
        
        // 清理资源
        void Cleanup();

        // 更新UBO数据
        void UpdateUBO(const std::string& name, const void* data);
        
        // 获取UBO绑定点
        GLuint GetUBOBindingPoint(const std::string& name) const;
        
        // 获取UBO缓冲区
        GLuint GetUBOBuffer(const std::string& name) const;

    private:
        struct UBOInfo
        {
            UBODescriptor descriptor;
            GLuint buffer;
        };

        std::unordered_map<std::string, UBOInfo> m_UBOs;
        bool m_Initialized = false;
    };
} 