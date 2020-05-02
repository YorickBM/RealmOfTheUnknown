#pragma once
#include <vector>
#include <map>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CollisionUtility {
public:
	static std::vector<glm::vec2> getClosesPointsInRange(int amountOfPoints, std::vector<glm::vec2> positions, glm::vec2 origin, float range) {
		std::map<float, glm::vec2> inRange = getRangePositions(positions, origin, range);
		std::vector<glm::vec2> closestPoints;
		if (inRange.size() > 3) {
			for (int i = 0; i < amountOfPoints; i++) {
			
				float index = findClosestKey(inRange, 0.f);
				closestPoints.push_back(inRange.at(index));
				inRange.erase(index);
			}
		}
		else {
			std::cout << "Empty Map Recieved" << std::endl;
		}

		return closestPoints;
	}

	static float getNewHeight(std::map<pair<float, float>, float>mapData, glm::vec2 origin, std::vector<glm::vec2> positions) {
		if (positions.size() != 0) {
			std::vector<glm::vec2> closePoints = CollisionUtility::getClosesPointsInRange(4, positions, origin, 10);

			//Point Calculations
			float y = mapData.at(make_pair(closePoints.at(0).x, closePoints.at(0).y));
			return y;
		}
		else { 
			std::cout << "Empty In Range Positions" << std::endl;
			return 2; 
		}

		
	}

	static std::map<float, glm::vec2> getRangePositions(std::vector<glm::vec2> positions, glm::vec2 origin, float range) {
		std::map<float, glm::vec2> inRange;

		for(glm::vec2 point : positions) {
			float distance = distanceBetweenTwoPoints(origin.x, origin.y, point.x, point.y);
			if (distance < range) inRange.insert(std::make_pair(distance, point));
		}

		return inRange;
	}

	static float distanceBetweenTwoPoints(float x, float y, float a, float b) {
		return sqrt(pow(x - a, 2) + pow(y - b, 2));
	}

	template <typename T1, typename T2>
	static T1 findClosestKey(const std::map<T1, T2>& data, T1 key)
	{
		if (data.size() == 0) {
			std::cout << "Received empty map" << std::endl;
			return 0;
			//throw std::out_of_range("Received empty map.");
		}

		auto lower = data.lower_bound(key);

		if (lower == data.end()) // If none found, return the last one.
			return std::prev(lower)->first;

		if (lower == data.begin())
			return lower->first;

		// Check which one is closest.
		auto previous = std::prev(lower);
		if ((key - previous->first) < (lower->first - key))
			return previous->first;

		return lower->first;
	}
};