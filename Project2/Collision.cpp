#include "Collision.h"
#include <time.h>

namespace que {
	Collision::Collision()
	{
	}
	Collision::~Collision()
	{
	}

	bool Collision::detectCollision(std::vector<Face> objectFaces, std::vector<glm::vec3> userVertices) {
		RayIntersect ray = RayIntersect();
		float zero = 0.f;

		clock_t tStart = clock();
		for (int current = 0; current < userVertices.size(); current++) {

			glm::vec3 vc = userVertices[current];    // c for "current"
			glm::vec3 CalcDir = glm::vec3(0, 0, 1);

			for (int currentF = 0; currentF < objectFaces.size(); currentF++) {
				Face fc = objectFaces[currentF];

				if (ray.rayTriangleIntersectMollerTrumBore(vc, CalcDir, fc.Pos0, fc.Pos1, fc.Pos2, zero, zero, zero)) {
					std::cout << "There is Collision" << std::endl;
					std::cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
					return true;
				}/// else std::cout << "There is No Collision" << std::endl;
			}
		}
		//std::cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
		return false;
	}

	bool Collision::detectCollision(std::vector<Face> objectFaces, std::vector<glm::vec3> userVertices, glm::vec3 PositionToCastTo) {
		RayIntersect ray = RayIntersect();
		float zero = 0.f;

		clock_t tStart = clock();
		for (int current = 0; current < userVertices.size(); current++) {

			glm::vec3 vc = userVertices[current];    // c for "current"
			glm::vec3 CalcDir = vc - PositionToCastTo;

			for (int currentF = 0; currentF < objectFaces.size(); currentF++) {
				Face fc = objectFaces[currentF];

				if (ray.rayTriangleIntersectMollerTrumBore(vc, CalcDir, fc.Pos0, fc.Pos1, fc.Pos2, zero, zero, zero)) {
					std::cout << "There is Collision" << std::endl;
					std::cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
					return true;
				}/// else std::cout << "There is No Collision" << std::endl;
			}
		}
		std::cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
		return false;
	}

	bool Collision::detectCollision(BoundingBox a, BoundingBox b)
	{
		return (a.min.x <= b.max.x && b.min.x <= a.max.x &&
			a.min.y <= b.max.y && b.min.y <= a.max.y &&
			a.min.z <= b.max.z && b.min.z <= a.max.z);
	}
	bool Collision::detectCollision(Sphere a, Sphere b)
	{
		glm::vec3 tmp = a.position - b.position; //Distance between centers
		return (glm::dot(tmp, tmp) <= glm::pow((a.radius + b.radius), 2));
	}
}
