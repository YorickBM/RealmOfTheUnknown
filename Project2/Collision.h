#pragma once
#include "CollisionExtra.h"
#include "RayIntersect.h"

// Std. Includes
#include <vector>
#include <iostream>
#include <list>
#include <math.h>
#include <algorithm> 

//GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define EPSILON  0.0000001
#define MODULUS(p) (sqrt(p.x*p.x + p.y*p.y + p.z*p.z))
#define TWOPI 6.283185307179586476925287
#define RTOD 57.2957795

namespace que {

	typedef struct {
		double x, y, z;
	} XYZ;

	class Collision
	{
	public:
		double CalcAngleSum(XYZ q, XYZ *p, int n)
		{
			int i;
			double m1, m2;
			double anglesum = 0, costheta;
			XYZ p1, p2;

			for (i = 0; i < n; i++) {

				p1.x = p[i].x - q.x;
				p1.y = p[i].y - q.y;
				p1.z = p[i].z - q.z;
				p2.x = p[(i + 1) % n].x - q.x;
				p2.y = p[(i + 1) % n].y - q.y;
				p2.z = p[(i + 1) % n].z - q.z;

				m1 = MODULUS(p1);
				m2 = MODULUS(p2);
				if (m1*m2 <= EPSILON)
					return(TWOPI); /* We are on a node, consider this inside */
				else
					costheta = (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z) / (m1*m2);

				anglesum += acos(costheta);
			}
			return(anglesum);
		}
		double CalcAngleSum(glm::vec3 q, std::vector<glm::vec3> p, int n)
		{
			int i;
			float m1, m2;
			float anglesum = 0, costheta = 0;
			glm::vec3 p1, p2;

			for (i = 0; i < n; i++) {

				p1.x = p[i].x - q.x;
				p1.y = p[i].y - q.y;
				p1.z = p[i].z - q.z;
				p2.x = p[(i + 1) % n].x - q.x;
				p2.y = p[(i + 1) % n].y - q.y;
				p2.z = p[(i + 1) % n].z - q.z;

				m1 = MODULUS(p1);
				m2 = MODULUS(p2);
				if (m1*m2 <= EPSILON) {
					std::cout << "Inside ?" << std::endl;
					return(TWOPI); /* We are on a node, consider this inside */
				}
				else
					costheta = (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z) / (m1*m2);

				anglesum += acos(costheta);
			}
			std::cout << costheta << std::endl;
			return(anglesum);
		}

		Collision();
		~Collision();

		bool detectCollision(std::vector<Face> objectFaces, std::vector<glm::vec3> userVertices);
		bool detectCollision(BoundingBox a, BoundingBox b);

		bool test(glm::vec3 da, glm::vec3 db, glm::vec3 dc) {
			glm::vec3 s = glm::dot(glm::cross(dc, db), glm::cross(da, db)) / glm::normalize(glm::cross(da, db));
			glm::vec3 t = glm::dot(glm::cross(dc, db), glm::cross(da, db)) / glm::normalize(glm::cross(da, db));

			if ((s.x >= 0 && s.x <= 1) && (t.x >= 0 && t.x <= 1))
			{
				glm::vec3 line = LineA;
				glm::vec3 res = (LineA + da * s);
				std::cout << "Collision?" << std::endl;
				std::cout << res.x << ";" << res.y << ";" << res.z << std::endl;
				return true;
			}
			std::cout << "No Collision?" << std::endl;
			return false;
		}

		bool test(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3, float d1, float d2, float d3) {
			float A = b1 - a1;
			float B = c1 - d1;
			float C = c1 - a1;
			float D = b2 - a2;
			float E = c2 - d2;
			float F = c2 - a2;

			//find t and s using formula
			float t = (C*E - F * B) / (E*A - B * D);
			float s = (D*C - A * F) / (D*B - A * E);

			//check if third equation is also satisfied(we have 3 equations and 2 variable
			if ((t*(b3 - a3) + s * (c3 - d3)) == c3 - a3)
				if (0 <= t <= 1 and 0 <= s <= 1)
					return true; // std::cout << "line segments intersect" << std::endl;
				else std::cout << "line segments intersect on being produced" << std::endl;

			return false;
		}

		glm::vec3 LineA, LineB, LineC, LineD;

	private:
		bool polyPoly(std::vector<glm::vec3> objectModel, std::vector<glm::vec3> userModel);
		bool polyLine(std::vector<glm::vec3> vertices, float x1, float y1, float z1, float x2, float y2, float z2);
		bool lineLine(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

		//True if collision is detected, false otherwise
		bool detectCollision(Sphere a, Sphere b);
	};
}

