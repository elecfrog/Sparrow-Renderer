/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "BaseDefinition.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Sparrow
{
    struct MeshComponent
    {
        UInt m_VertexCount;
        UInt m_IndexCount;
        Vector<Vector3> m_PositionBuffer;
        Vector<Vector3> m_NormalsBuffer;
        Vector<Vector3> m_TangentsBuffer;
        Vector<Vector2> m_TexCoord0Buffer;
        Vector<UInt> m_IndicesBuffer;
    };
} // Sparrow
