#pragma once
#include "glm.h"

namespace gw {

	// thanks to http://www.gamedev.net/topic/619296-ultra-fast-2d-aabb-overlap-test/
	// TODO: Solution for 16-byte aligned Box class (MMX instructions need that) for both gcc and msvc
	//struct Box {
	//	Box() {}
	//	Box(float xmin, float xmax, float ymin, float ymax)
	//	{
	//		v1[0] = xmin; v1[1] = -xmax; v1[2] = ymin; v1[3] = -ymax;
	//		v2[0] = xmax; v2[1] = -xmin; v2[2] = ymax; v2[3] = -ymin;
	//	}
	//	float v1[4];
	//	float v2[4];
	//} __attribute__ ((aligned (16)));


	inline bool BoxIntersect(const glm::vec4 &a, const glm::vec4 &b) {
		// Returns true if rectangles a and b overlap
		//		a.x - xMin		b.x - xMin
		//		a.y - yMin		b.y - yMin
		//		a.z - xMax		b.z - xMax
		//		a.w - yMax		b.w - yMax

		// TODO
		//  - make a and b parameters Boxes instead of vec4s
		//  - test this (correct results)
		//  - time this (in optimized gcc-generated code and msvc generated code)
		//  - big question - is this actually faster than writing naive solution and letting compiler optimize??
		//  - avoid conversion to box shen calling this function
		//  - what about AVX instrcutions

		__m128 min = _mm_set_ps(a.x, b.x, a.y, b.y);
		__m128 max = _mm_set_ps(b.z, a.z, b.w, a.w);
		__m128 cmp = _mm_cmple_ps(min, max);

		return _mm_movemask_ps(cmp) == 0xf;
	}

}