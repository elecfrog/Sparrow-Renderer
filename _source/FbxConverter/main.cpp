#include <format>
#include <fbxsdk.h>
#include "FbxScene.h"

int main(int argc, char** argv)
{
    // 初始化FBX SDK
    FbxManager* fbxManager = FbxManager::Create();
    if (!fbxManager)
    {
        std::println("Error: Unable to create FBX Manager!");
        return 1;
    }

    // 创建IO设置对象
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);

    // 测试FbxScene
    FbxScene scene;
    scene.Initialize();

    // 清理
    fbxManager->Destroy();

    return 0;
}
