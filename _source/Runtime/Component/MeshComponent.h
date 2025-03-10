/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "BaseDefinition.h"

namespace Sparrow
{
    struct MeshComponent
    {
        UInt m_VertexCount;
        UInt m_IndexCount;
        Vector<Vector3f> m_PositionBuffer;
        Vector<Vector3f> m_NormalsBuffer;
        Vector<Vector3f> m_TangentsBuffer;
        Vector<Vector2f> m_TexCoord0Buffer;
        Vector<UInt> m_IndicesBuffer;
    };
} // Sparrow
