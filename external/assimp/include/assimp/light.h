/** @file light.h
 *  @brief Defines the aiLight data structure
 */

#pragma once
#ifndef AI_LIGHT_H_INC
#define AI_LIGHT_H_INC

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include <assimp/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
/** Enumerates all supported types of light sources.
 */
enum aiLightSourceType {
    aiLightSource_UNDEFINED     = 0x0,

    //! A directional light source has a well-defined direction
    //! but is infinitely far away. That's quite a good
    //! approximation for sun light.
    aiLightSource_DIRECTIONAL   = 0x1,

    //! A point light source has a well-defined position
    //! in space but no direction - it emits light in all
    //! directions. A normal bulb is a point light.
    aiLightSource_POINT         = 0x2,

    //! A spot light source emits light in a specific
    //! angle. It has a position and a direction it is pointing to.
    //! A good example for a spot light is a light spot in
    //! sport arenas.
    aiLightSource_SPOT          = 0x3,

    //! The generic light level of the world, including the bounces
    //! of all other light sources.
    //! Typically, there's at most one ambient light in a scene.
    //! This light type doesn't have a valid position, direction, or
    //! other properties, just a color.
    aiLightSource_AMBIENT       = 0x4,

    //! An area light is a rectangle with predefined size that uniformly
    //! emits light from one of its sides. The position is center of the
    //! rectangle and direction is its normal vector.
    aiLightSource_AREA          = 0x5,

    /** This value is not used. It is just there to force the
     *  compiler to map this enum to a 32 Bit integer.
     */
#ifndef SWIG
    _aiLightSource_Force32Bit = INT_MAX
#endif
};

// ---------------------------------------------------------------------------
/** Helper structure to describe a light source.
 *
 *  Assimp supports multiple sorts of light sources, including
 *  directional, point and spot lights. All of them are defined with just
 *  a single structure and distinguished by their parameters.
 *  Note - some file formats (such as 3DS, ASE) export a "target point" -
 *  the point a spot light is looking at (it can even be animated). Assimp
 *  writes the target point as a sub-node of a spot-lights's main node,
 *  called "<spotName>.Target". However, this is just additional information
 *  then, the transformation tracks of the main node make the
 *  spot light already point in the right direction.
*/
struct aiLight {
    /** The name of the light source.
     *
     *  There must be a node in the scene-graph with the same name.
     *  This node specifies the position of the light in the scene
     *  hierarchy and can be animated.
     */
    C_STRUCT aiString mName;

    /** The type of the light source.
     *
     * aiLightSource_UNDEFINED is not a valid value for this member.
     */
    C_ENUM aiLightSourceType mType;

    /** Position of the light source in space. Relative to the
     *  transformation of the node corresponding to the light.
     *
     *  The position is undefined for directional lights.
     */
    C_STRUCT aiVector3D mPosition;

    /** Direction of the light source in space. Relative to the
     *  transformation of the node corresponding to the light.
     *
     *  The direction is undefined for point lights. The vector
     *  may be normalized, but it needn't.
     */
    C_STRUCT aiVector3D mDirection;

    /** Up direction of the light source in space. Relative to the
     *  transformation of the node corresponding to the light.
     *
     *  The direction is undefined for point lights. The vector
     *  may be normalized, but it needn't.
     */
    C_STRUCT aiVector3D mUp;

    /** Constant light attenuation factor.
     *
     *  The intensity of the light source at a given distance 'd' from
     *  the light's position is
     *  @code
     *  Atten = 1/( att0 + att1 * d + att2 * d*d)
     *  @endcode
     *  This member corresponds to the att0 variable in the equation.
     *  Naturally undefined for directional lights.
     */
    float mAttenuationConstant;

    /** Linear light attenuation factor.
     *
     *  The intensity of the light source at a given distance 'd' from
     *  the light's position is
     *  @code
     *  Atten = 1/( att0 + att1 * d + att2 * d*d)
     *  @endcode
     *  This member corresponds to the att1 variable in the equation.
     *  Naturally undefined for directional lights.
     */
    float mAttenuationLinear;

    /** Quadratic light attenuation factor.
     *
     *  The intensity of the light source at a given distance 'd' from
     *  the light's position is
     *  @code
     *  Atten = 1/( att0 + att1 * d + att2 * d*d)
     *  @endcode
     *  This member corresponds to the att2 variable in the equation.
     *  Naturally undefined for directional lights.
     */
    float mAttenuationQuadratic;

    /** Diffuse color of the light source
     *
     *  The diffuse light color is multiplied with the diffuse
     *  material color to obtain the final color that contributes
     *  to the diffuse shading term.
     */
    C_STRUCT aiColor3D mColorDiffuse;

    /** Specular color of the light source
     *
     *  The specular light color is multiplied with the specular
     *  material color to obtain the final color that contributes
     *  to the specular shading term.
     */
    C_STRUCT aiColor3D mColorSpecular;

    /** Ambient color of the light source
     *
     *  The ambient light color is multiplied with the ambient
     *  material color to obtain the final color that contributes
     *  to the ambient shading term. Most renderers will ignore
     *  this value it, is just a remaining of the fixed-function pipeline
     *  that is still supported by quite many file formats.
     */
    C_STRUCT aiColor3D mColorAmbient;

    /** Inner angle of a spot light's light cone.
     *
     *  The spot light has maximum influence on objects inside this
     *  angle. The angle is given in radians. It is 2PI for point
     *  lights and undefined for directional lights.
     */
    float mAngleInnerCone;

    /** Outer angle of a spot light's light cone.
     *
     *  The spot light does not affect objects outside this angle.
     *  The angle is given in radians. It is 2PI for point lights and
     *  undefined for directional lights. The outer angle must be
     *  greater than or equal to the inner angle.
     *  It is assumed that the application uses a smooth
     *  interpolation between the inner and the outer cone of the
     *  spot light.
     */
    float mAngleOuterCone;

    /** Size of area light source. */
    C_STRUCT aiVector2D mSize;

#ifdef __cplusplus

    aiLight() AI_NO_EXCEPT
        :   mType                 (aiLightSource_UNDEFINED)
        ,   mAttenuationConstant  (0.f)
        ,   mAttenuationLinear    (1.f)
        ,   mAttenuationQuadratic (0.f)
        ,   mAngleInnerCone       ((float)AI_MATH_TWO_PI)
        ,   mAngleOuterCone       ((float)AI_MATH_TWO_PI)
        ,   mSize                 (0.f, 0.f)
    {
    }

#endif
};

#ifdef __cplusplus
}
#endif

#endif // !! AI_LIGHT_H_INC
