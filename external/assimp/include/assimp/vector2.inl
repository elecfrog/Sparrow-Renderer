/** @file  vector2.inl
 *  @brief Inline implementation of aiVector2t<TReal> operators
 */
#pragma once
#ifndef AI_VECTOR2D_INL_INC
#define AI_VECTOR2D_INL_INC

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#ifdef __cplusplus
#include <assimp/vector2.h>

#include <cmath>

// ------------------------------------------------------------------------------------------------
template <typename TReal>
template <typename TOther>
aiVector2t<TReal>::operator aiVector2t<TOther> () const {
    return aiVector2t<TOther>(static_cast<TOther>(x),static_cast<TOther>(y));
}
// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
void aiVector2t<TReal>::Set( TReal pX, TReal pY) {
    x = pX; y = pY;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
TReal aiVector2t<TReal>::SquareLength() const {
    return x*x + y*y;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
TReal aiVector2t<TReal>::Length() const {
    return std::sqrt( SquareLength());
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
aiVector2t<TReal>& aiVector2t<TReal>::Normalize() {
    *this /= Length();
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
const aiVector2t<TReal>& aiVector2t<TReal>::operator += (const aiVector2t& o) {
    x += o.x; y += o.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
const aiVector2t<TReal>& aiVector2t<TReal>::operator -= (const aiVector2t& o) {
    x -= o.x; y -= o.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
const aiVector2t<TReal>& aiVector2t<TReal>::operator *= (TReal f) {
    x *= f; y *= f;
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
const aiVector2t<TReal>& aiVector2t<TReal>::operator /= (TReal f) {
    x /= f; y /= f;
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
TReal aiVector2t<TReal>::operator[](unsigned int i) const {
	switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			break;

    }
    return x;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
bool aiVector2t<TReal>::operator== (const aiVector2t& other) const {
    return x == other.x && y == other.y;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
bool aiVector2t<TReal>::operator!= (const aiVector2t& other) const {
    return x != other.x || y != other.y;
}

// ---------------------------------------------------------------------------
template<typename TReal>
inline
bool aiVector2t<TReal>::Equal(const aiVector2t& other, TReal epsilon) const {
    return
        std::abs(x - other.x) <= epsilon &&
        std::abs(y - other.y) <= epsilon;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
aiVector2t<TReal>& aiVector2t<TReal>::operator= (TReal f)   {
    x = y = f;
    return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
inline
const aiVector2t<TReal> aiVector2t<TReal>::SymMul(const aiVector2t& o) {
    return aiVector2t(x*o.x,y*o.y);
}


// ------------------------------------------------------------------------------------------------
// symmetric addition
template <typename TReal>
inline
aiVector2t<TReal> operator + (const aiVector2t<TReal>& v1, const aiVector2t<TReal>& v2) {
    return aiVector2t<TReal>( v1.x + v2.x, v1.y + v2.y);
}

// ------------------------------------------------------------------------------------------------
// symmetric subtraction
template <typename TReal>
inline
aiVector2t<TReal> operator - (const aiVector2t<TReal>& v1, const aiVector2t<TReal>& v2) {
    return aiVector2t<TReal>( v1.x - v2.x, v1.y - v2.y);
}

// ------------------------------------------------------------------------------------------------
// scalar product
template <typename TReal>
inline
TReal operator * (const aiVector2t<TReal>& v1, const aiVector2t<TReal>& v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

// ------------------------------------------------------------------------------------------------
// scalar multiplication
template <typename TReal>
inline
aiVector2t<TReal> operator * ( TReal f, const aiVector2t<TReal>& v) {
    return aiVector2t<TReal>( f*v.x, f*v.y);
}

// ------------------------------------------------------------------------------------------------
// and the other way around
template <typename TReal>
inline
aiVector2t<TReal> operator * ( const aiVector2t<TReal>& v, TReal f) {
    return aiVector2t<TReal>( f*v.x, f*v.y);
}

// ------------------------------------------------------------------------------------------------
// scalar division
template <typename TReal>
inline
aiVector2t<TReal> operator / ( const aiVector2t<TReal>& v, TReal f) {
    return v * (1/f);
}

// ------------------------------------------------------------------------------------------------
// vector division
template <typename TReal>
inline
aiVector2t<TReal> operator / ( const aiVector2t<TReal>& v, const aiVector2t<TReal>& v2) {
    return aiVector2t<TReal>(v.x / v2.x,v.y / v2.y);
}

// ------------------------------------------------------------------------------------------------
// vector negation
template <typename TReal>
inline
aiVector2t<TReal> operator - ( const aiVector2t<TReal>& v) {
    return aiVector2t<TReal>( -v.x, -v.y);
}

#endif

#endif // AI_VECTOR2D_INL_INC
