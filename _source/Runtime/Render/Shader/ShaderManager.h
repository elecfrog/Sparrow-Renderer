/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "BaseDefinition.h"
#include "Render/RHI/Shader.h"

namespace Sparrow
{
    class ShaderManager
    {
    public:
        ShaderManager() = default;
        ~ShaderManager() = default;
        ShaderManager(const ShaderManager&) = delete;
        ShaderManager(ShaderManager&&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;
        ShaderManager& operator=(ShaderManager&&) = delete;

        Bool LoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            return true;
        }

        Bool ReloadLoadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            return true;
        }

    private:
        HashMap<String, Shader*> m_ShaderMap;
    };
} // Sparrow
