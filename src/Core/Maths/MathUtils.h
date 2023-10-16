#pragma once

namespace Maths
{

#undef M_PI
	static const float M_PI              = 3.14159265358979323846264338327950288f;
	static const float M_HALF_PI         = M_PI * 0.5f;
	static const int M_MIN_INT           = 0x80000000;
	static const int M_MAX_INT           = 0x7fffffff;
	static const unsigned M_MIN_UNSIGNED = 0x00000000;
	static const unsigned M_MAX_UNSIGNED = 0xffffffff;

	static const float M_EPSILON       = 0.000001f;
	static const float M_LARGE_EPSILON = 0.00005f;
	static const float M_MIN_NEARCLIP  = 0.01f;
	static const float M_MAX_FOV       = 160.0f;
	static const float M_LARGE_VALUE   = 100000000.0f;
	//  static const float M_INFINITY      = (float)HUGE_VAL;
	static const float M_DEGTORAD      = M_PI / 180.0f;
	static const float M_DEGTORAD_2    = M_PI / 360.0f; // M_DEGTORAD / 2.f
	static const float M_RADTODEG      = 1.0f / M_DEGTORAD;

} // namespace Maths

