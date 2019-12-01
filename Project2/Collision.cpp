#include "Collision.h"

Collision::Collision()
{
}
Collision::~Collision()
{
}

void Collision::detectCollision(std::vector<glm::vec3> objectVertices, std::vector<glm::vec3> userVertices) {
	if (polyPoly(objectVertices, userVertices)) std::cout << "There is Collision" << std::endl;
	else std::cout << "There is Collision no" << std::endl;
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
bool Collision::lineLine(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4) {
	float Vector1 = sqrt(pow((x1 - x3)*((y1 - y2)*(z3 - z2) - (z1 - z2)*(y3 - y4)), 2) +
		pow((y1 - y3)*((z1 - z3)*(x3 - x4) - (x1 - x2)*(z3 - z4)), 2) +
		pow((z1 - z3)*((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4)), 2));
	float Vector2 = sqrt(pow((y1 - y2)*(z3 - z4) - (z1 - z2)*(y3 - y4), 2) +
		pow((z1 - z2)*(x3 - x4) - (x1 - x2)*(z3 - z4), 2) +
		pow((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4), 2));

	float FinishedProduct = Vector1 / Vector2;
	if (FinishedProduct == 0) return true;

	/* 2 Dimensional Vector Collision
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
