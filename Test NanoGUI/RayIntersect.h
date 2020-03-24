#pragma once
#include <cstdio> 
#include <cstdlib> 
#include <memory> 
#include <vector> 
#include <utility> 
#include <cstdint> 
#include <iostream> 
#include <fstream> 
#include <cmath> 
#include <limits> 
#include <random> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

constexpr float kEpsilon = 1e-8;

class RayIntersect
{
public:
	RayIntersect() {

	}
	~RayIntersect() {

	}

	bool rayTriangleIntersectMollerTrumBore(
		const glm::vec3 &orig, const glm::vec3 &dir,
		const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
		float &t, float &u, float &v)
	{
		glm::vec3 v0v1 = v1 - v0;
		glm::vec3 v0v2 = v2 - v0;
		///glm::vec3 pvec = dir.crossProduct(v0v2);
		///float det = v0v1.dotProduct(pvec);
		glm::vec3 pvec = glm::cross(dir, v0v2);
		float det = glm::dot(v0v1, pvec);

		#ifdef CULLING 
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < kEpsilon) return false;
		#else 
		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < kEpsilon) return false;
		#endif 

		float invDet = 1 / det;

		glm::vec3 tvec = orig - v0;
		//u = tvec.dotProduct(pvec) * invDet;
		u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		///glm::vec3 qvec = tvec.crossProduct(v0v1);
		///v = dir.dotProduct(qvec) * invDet;
		glm::vec3 qvec = glm::cross(tvec, v0v1);
		v = glm::dot(dir, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		///t = v0v2.dotProduct(qvec) * invDet;
		t = glm::dot(v0v2, qvec) * invDet;

		return true;
	}

	bool rayTriangleIntersect(
		const glm::vec3 &orig, const glm::vec3 &dir,
		const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
		float &t, float &u, float &v)
	{
		// compute plane's normal
		glm::vec3 v0v1 = v1 - v0;
		glm::vec3 v0v2 = v2 - v0;
		// no need to normalize
		///glm::vec3 N = v0v1.crossProduct(v0v2); // N 
		///float denom = N.dotProduct(N);

		glm::vec3 N = glm::cross(v0v1, v0v2);
		float denom = glm::dot(N,N);

		// Step 1: finding P

		// check if ray and plane are parallel ?
		///float NdotRayDirection = N.dotProduct(dir);
		float NdotRayDirection = glm::dot(N, dir);
		if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
			return false; // they are parallel so they don't intersect ! 

		// compute d parameter using equation 2
		///float d = N.dotProduct(v0);
		float d = glm::dot(N, v0);

		// compute t (equation 3)
		///t = (N.dotProduct(orig) + d) / NdotRayDirection;
		t = (glm::dot(N, orig) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) return false; // the triangle is behind 

		// compute the intersection point using equation 1
		glm::vec3 P = orig + t * dir;

		// Step 2: inside-outside test
		glm::vec3 C; // vector perpendicular to triangle's plane 

		// edge 0
		glm::vec3 edge0 = v1 - v0;
		glm::vec3 vp0 = P - v0;
		///C = edge0.crossProduct(vp0);
		///if (N.dotProduct(C) < 0) return false; // P is on the right side 
		C = glm::cross(edge0, vp0);
		if (glm::dot(N, C) < 0) return false;

		// edge 1
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 vp1 = P - v1;
		///C = edge1.crossProduct(vp1);
		///if ((u = N.dotProduct(C)) < 0)  return false; // P is on the right side 
		C = glm::cross(edge1, vp1);
		if (( u = glm::dot(N, C)) < 0) return false;

		// edge 2
		glm::vec3 edge2 = v0 - v2;
		glm::vec3 vp2 = P - v2;
		///C = edge2.crossProduct(vp2);
		///if ((v = N.dotProduct(C)) < 0) return false; // P is on the right side; 
		C = glm::cross(edge2, vp2);
		if (( v = glm::dot(N, C)) < 0) return false;

		u /= denom;
		v /= denom;

		return true; // this ray hits the triangle 
	}
};

