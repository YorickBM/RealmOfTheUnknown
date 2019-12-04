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
		//if (polyPoly(objectVertices, userVertices)) std::cout << "There is Collision" << std::endl;
		//else std::cout << "There is Collision no" << std::endl;

		RayIntersect ray = RayIntersect();
		float zero = 0.f;

		clock_t tStart = clock();
		for (int current = 0; current < userVertices.size(); current++) {

			glm::vec3 vc = userVertices[current];    // c for "current"
			glm::vec3 CalcDir = glm::vec3(0,0,1);

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
	/*void Collision::detectCollision(Model Object, Model User) {
		if (detectCollision(Object.getBoundingBox(), User.getBoundingBox())) { //There is Collision
			std::vector<glm::vec3> objectVertices;
			std::vector<glm::vec3> userVertices;

			//Put al Object Vertices into a vector list
			for (int i = 0; i < Object.getMeshes().size(); i++) {
				for (int z = 0; z < Object.getMeshes().at(i).vertices.size(); z++) {
					glm::vec3 pos = Object.getMeshes().at(i).vertices.at(z).Position;
					objectVertices.push_back(pos);
				}
			}
			//Put al User Vertices into a vector list
			for (int i = 0; i < User.getMeshes().size(); i++) {
				for (int z = 0; z < User.getMeshes().at(i).vertices.size(); z++) {
					glm::vec3 pos = User.getMeshes().at(i).vertices.at(z).Position;
					userVertices.push_back(pos);
				}
			}

			//Check if any of the lines between a vertice collides with another
			if (polyPoly(objectVertices, userVertices)) {
				std::cout << "There is Collision" << std::endl;
			}
		} // There is No Collision between 2 bounding boxes (Scene Model & User Model)
	}
	*/
	bool Collision::polyPoly(std::vector<glm::vec3> objectModel, std::vector<glm::vec3> userModel) {

		// go through each of the vertices, plus the next
		// vertex in the list
		int next = 0;
		for (int current = 0; current < objectModel.size(); current++) {

			// get next vertex in list
			// if we've hit the end, wrap around to 0
			next = current + 1;
			if (next == objectModel.size()) next = 0;

			// get the Vectors at our current position
			// this makes our if statement a little cleaner
			glm::vec3 vc = objectModel[current];    // c for "current"
			glm::vec3 vn = objectModel[next];       // n for "next"

			// now we can use these two points (a line) to compare
			// to the other polygon's vertices using polyLine()
			bool collision = polyLine(userModel, vc.x, vc.y, vc.z, vn.x, vn.y, vn.z);
			if (collision) return true;
			// optional: check if the 2nd polygon is INSIDE the first
			//collision = polyPoint(p1, p2[0].x, p2[0].y);
			//if (collision) return true;
		}

		return false;
	}
	bool Collision::polyLine(std::vector<glm::vec3> vertices, float x1, float y1, float z1, float x2, float y2, float z2) {

		// go through each of the vertices, plus the next
		// vertex in the list
		int next = 0;
		for (int current = 0; current < vertices.size(); current++) {

			// get next vertex in list
			// if we've hit the end, wrap around to 0
			next = current + 1;
			if (next == vertices.size()) next = 0;

			// get the PVectors at our current position
			// extract X/Y coordinates from each
			float x3 = vertices[current].x;
			float y3 = vertices[current].y;
			float z3 = vertices[current].z;
			float x4 = vertices[next].x;
			float y4 = vertices[next].y;
			float z4 = vertices[next].z;

			// do a Line/Line comparison
			// if true, return 'true' immediately and
			// stop testing (faster)
			bool hit = lineLine(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
			if (hit) {
				return true;
			}
		}

		// never got a hit
		return false;
	}
	bool Collision::lineLine(
		float x0, float y0, float z0,
		float x, float y, float z,
		float x02, float y02, float z02,
		float x2, float y2, float z2
	) {

		if (this->test(x0, y0, z0, x, y, z, x02, y02, z02, x2, y2, z2)) return true;

		/*glm::vec3 LineA = glm::vec3(x0, y0, z0);
		glm::vec3 LineB = glm::vec3(x, y, z);
		glm::vec3 LineC = glm::vec3(x02, y02, z02);
		glm::vec3 LineD = glm::vec3(x2, y2, z2);

		this->LineA = LineA;
		this->LineB = LineB;
		this->LineC = LineC;
		this->LineD = LineD;

		if (this->test((LineB - LineA), (LineD - LineC), (LineC - LineA))) {
			std::cout << "APos-1:" << x0 << "," << y0 << "," << z0 << std::endl;
			std::cout << "APos-2:" << x << "," << y << "," << z << std::endl;
			std::cout << "BPos-1:" << x02 << "," << y02 << "," << z02 << std::endl;
			std::cout << "BPos-2:" << x2 << "," << y2 << "," << z2 << std::endl;
			return true;
		}
		/*
		float Vector1 = sqrt(pow(((x0 - x02)*((y0 - y)*(z02 - z) - (z0 - z)*(y02 - y2))), 2) +
			pow(((y0 - y02)*((z0 - z)*(x02 - x2) - (x0 - x)*(z02 - z2))), 2) +
			pow(((z0 - z02)*((x0 - x)*(y02 - y2) - (y0 - y)*(x02 - x2))), 2));
		float Vector2 = sqrt(pow(((y0 - y)*(z02 - z2) - (z0 - z)*(y02 - y2)), 2) +
			pow(((z0 - z)*(x02 - x2) - (x0 - x)*(z02 - z2)), 2) +
			pow(((x0 - x)*(y02 - y2) - (y0 - y)*(x02 - x2)), 2));

		float FinishedProduct = Vector1 / Vector2;
		if (FinishedProduct == 0) {
			std::cout << "APos-1:" << x0 << "," << y0 << "," << z0 << std::endl;
			std::cout << "APos-2:" << x << "," << y << "," << z << std::endl;
			std::cout << "BPos-1:" << x02 << "," << y02 << "," << z02 << std::endl;
			std::cout << "BPos-2:" << x2 << "," << y2 << "," << z2 << std::endl;
			return true;
		}

		 2 Dimensional Vector Collision
		// calculate the direction of the lines
		float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
		float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

		// if uA and uB are between 0-1, lines are colliding
		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
			return true;
		}*/
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
