//#include "NodeParser.h"
//
//namespace Sparrow
//{
//    struct TransformData
//    {
//        FbxDouble3 translation;
//        FbxDouble3 preRotation;    // 预旋转
//        FbxDouble3 rotation;       // 主旋转
//        FbxDouble3 postRotation;   // 后旋转
//        FbxEuler::EOrder rotationOrder;  // 旋转顺序
//        FbxDouble3 scaling;
//        FbxMatrix localTransform;
//        FbxMatrix globalTransform;
//    };
//
//    std::shared_ptr<NodeData> NodeParser::ParseScene(FbxScene* scene)
//    {
//        if (!scene)
//        {
//            return nullptr;
//        }
//
//        m_AllNodes.clear();
//        return ParseNode(scene->GetRootNode());
//    }
//
//    std::shared_ptr<NodeData> NodeParser::ParseNode(FbxNode* node)
//    {
//        if (!node)
//        {
//            return nullptr;
//        }
//
//        auto nodeData = std::make_shared<NodeData>();
//        
//        // 获取节点名称
//        nodeData->name = node->GetName();
//
//        // 处理节点属性和变换
//        ProcessNodeAttributes(node, nodeData);
//        ProcessTransform(node, nodeData);
//
//        // 存储节点
//        m_AllNodes.push_back(nodeData);
//
//        // 递归处理子节点
//        const int childCount = node->GetChildCount();
//        for (int i = 0; i < childCount; i++)
//        {
//            auto childData = ParseNode(node->GetChild(i));
//            if (childData)
//            {
//                nodeData->children.push_back(childData);
//            }
//        }
//
//        return nodeData;
//    }
//
//    void NodeParser::ProcessNodeAttributes(FbxNode* node, std::shared_ptr<NodeData>& nodeData)
//    {
//        // 获取属性数量
//        const int attrCount = node->GetNodeAttributeCount();
//        
//        // 遍历所有属性
//        for (int i = 0; i < attrCount; i++)
//        {
//            FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
//            if (attribute)
//            {
//                NodeAttributeData attrData;
//                attrData.type = attribute->GetAttributeType();
//                attrData.name = attribute->GetName();
//                attrData.attribute = attribute;
//                
//                nodeData->attributes.push_back(attrData);
//            }
//        }
//    }
//
//    void NodeParser::ProcessTransform(FbxNode* node, std::shared_ptr<NodeData>& nodeData)
//    {
//        auto& transform = nodeData->transform;
//
//        // 获取位移
//        transform.translation = node->LclTranslation.Get();
//
//        // 获取旋转
//        transform.preRotation = node->PreRotation.Get();
//        transform.rotation = node->LclRotation.Get();
//        transform.postRotation = node->PostRotation.Get();
//        transform.rotationOrder = node->RotationOrder.Get();
//
//        // 获取缩放
//        transform.scaling = node->LclScaling.Get();
//
//        // 计算局部变换矩阵
//        FbxAMatrix translationM, rotationM, preRotationM, postRotationM, scalingM;
//        
//        translationM.SetT(transform.translation);
//        preRotationM.SetR(transform.preRotation, transform.rotationOrder);
//        rotationM.SetR(transform.rotation, transform.rotationOrder);
//        postRotationM.SetR(transform.postRotation, transform.rotationOrder);
//        scalingM.SetS(transform.scaling);
//
//        // 组合变换矩阵：Scale * PreRotation * Rotation * PostRotation * Translation
//        transform.localTransform = scalingM * preRotationM * rotationM * postRotationM * translationM;
//        
//        // 获取全局变换
//        transform.globalTransform = node->EvaluateGlobalTransform();
//    }
//
//    std::vector<std::shared_ptr<NodeData>> NodeParser::GetNodesByAttributeType(FbxNodeAttribute::EType type) const
//    {
//        std::vector<std::shared_ptr<NodeData>> result;
//        
//        for (const auto& node : m_AllNodes)
//        {
//            if (node->HasAttributeType(type))
//            {
//                result.push_back(node);
//            }
//        }
//        
//        return result;
//    }
//} 