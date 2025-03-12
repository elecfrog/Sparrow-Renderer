#pragma once

#include <algorithm>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include <memory>
#include <any>

namespace Sparrow
{
    // Forward declarations
    struct NodeAttributeData
    {
        FbxNodeAttribute::EType type;
        std::string name;
        FbxNodeAttribute* attribute;  // 原始FBX属性指针，用于后续具体类型的处理
    };

    struct NodeData
    {
        std::string name;
        std::vector<NodeAttributeData> attributes;  // 存储所有属性
        FbxDouble3 translation;
        FbxDouble3 rotation;
        FbxDouble3 scaling;
        FbxMatrix globalTransform;
        std::vector<std::shared_ptr<NodeData>> children;

        // 辅助函数：检查是否包含特定类型的属性
        bool HasAttributeType(FbxNodeAttribute::EType type) const
        {
            return std::any_of(attributes.begin(), attributes.end(),
                [type](const NodeAttributeData& attr) { return attr.type == type; });
        }

        // 获取特定类型的所有属性
        std::vector<NodeAttributeData> GetAttributesByType(FbxNodeAttribute::EType type) const
        {
            std::vector<NodeAttributeData> result;
            for (const auto& attr : attributes)
            {
                if (attr.type == type)
                {
                    result.push_back(attr);
                }
            }
            return result;
        }
    };

    class NodeParser
    {
    public:
        NodeParser() = default;
        ~NodeParser() = default;

        // Parse the entire scene and return root node
        std::shared_ptr<NodeData> ParseScene(FbxScene* scene);
        
        // Get all nodes that have a specific attribute type
        std::vector<std::shared_ptr<NodeData>> GetNodesByAttributeType(FbxNodeAttribute::EType type) const;

    private:
        // Parse a single node and its children recursively
        std::shared_ptr<NodeData> ParseNode(FbxNode* node);
        
        // Process node attributes
        void ProcessNodeAttributes(FbxNode* node, std::shared_ptr<NodeData>& nodeData);
        
        // Process node transform
        void ProcessTransform(FbxNode* node, std::shared_ptr<NodeData>& nodeData);

        // Store all parsed nodes for quick access
        std::vector<std::shared_ptr<NodeData>> m_AllNodes;
    };
} 