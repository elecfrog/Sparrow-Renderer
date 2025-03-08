#pragma once

#include "Base/BaseDefinition.h"

namespace Sparrow
{
    struct Light
    {
        Vector3f position = Vector3f(0.0f, 0.836f, 2.889f);
        Vector3f color = Vector3f(1.f, 1.f, 1.f);
        Vector3f ambient_color = Vector3f(0.5f, 0.5f, 0.5f);
    };

    // struct PointLight
    // {
    //     Vector3f position;
    //     // radiant flux in W
    //     Vector3f flux;
    //
    //     // calculate an appropriate radius for light culling
    //     // a windowing function in the shader will perform a smooth transition to zero
    //     // this is not physically based and usually artist controlled
    //     Float calculateRadius() const
    //     {
    //         // radius = where attenuation would lead to an intensity of 1W/m^2
    //         const Float INTENSITY_CUTOFF = 1.0f;
    //         const Float ATTENTUATION_CUTOFF = 0.05f;
    //         Vector3f intensity = flux / (4.0f * Math::PI);
    //         Float maxIntensity = intensity.MaxElement();
    //         Float attenuation = Math::Max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
    //         return 1.0f / Math::Sqrt(attenuation);
    //     }
    //
    //     void ComputeDirection(const Vector3f& objPosition)
    //     {
    //         Vector3f direction = position - objPosition;
    //     }
    // };
    //
    // struct AmbientLight
    // {
    //     Vector3f irradiance;
    // };
    //
    // struct DirectionalLight
    // {
    //     Vector3f direction;
    //     Vector3f color;
    // };
    //
    // struct LightList
    // {
    //     // vertex buffers seem to be aligned to 16 bytes
    //     struct PointLightVertex
    //     {
    //         Vector3f position;
    //         Float padding;
    //         // radiant intensity in W/sr
    //         // can be calculated from radiant flux
    //         Vector3f intensity;
    //         Float radius;
    //     };
    // };
    //
    // class PointLightList : public LightList
    // {
    // public:
    //     void Init() {}
    //     void Shutdown() {}
    //
    //     // upload changes to GPU
    //     void Update() {}
    //
    //     std::vector<PointLight> lights;
    //     std::shared_ptr<BufferData> buffer;
    // };
}
