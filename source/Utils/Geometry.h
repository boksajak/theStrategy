#pragma once
#include "glm.h"

namespace gw {

	struct AABB2D {
		AABB2D() : minPoint(FLT_MAX), maxPoint(FLT_MIN) { }

		AABB2D(glm::vec2 minPoint, glm::vec2 maxPoint) : minPoint(minPoint), maxPoint(maxPoint) { }

		glm::vec2 minPoint;
		glm::vec2 maxPoint;

		glm::vec2 size() {
			return maxPoint - minPoint;
		}

	};

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

	// Point-AABB
	inline bool BoxIntersect(const glm::vec2 &p, const AABB2D &aabb) {
		return (p.x > aabb.minPoint.x && p.x < aabb.maxPoint.x && p.y > aabb.minPoint.y && p.y < aabb.maxPoint.y);
	}

		// AABB-AABB
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

		// return !((b.z<a.x || b.x>a.z) || (b.w<a.y || b.y>a.w))
		__m128 min = _mm_set_ps(a.x, b.x, a.y, b.y);
		__m128 max = _mm_set_ps(b.z, a.z, b.w, a.w);
		__m128 cmp = _mm_cmple_ps(min, max);

		return _mm_movemask_ps(cmp) == 0xf;
	}

	inline glm::vec2 normalizedToAbsoulte(const glm::vec2 &normalized, const glm::vec2 &screenSize) {
		return screenSize * normalized;
	}

	inline glm::vec2 absoulteToNormalized(const glm::vec2 &absolute, const glm::vec2 &screenSize) {
		return absolute / screenSize;
	}
}