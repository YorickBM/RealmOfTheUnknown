#pragma once
#include <fstream>

#include <map>
#include <vector>

#include "Mesh.h"
#include "ModelLoader.h";
#include "AnimModel.h"

struct Chunk {
	AnimModel model;
};

class ChunkManager
{
public:
	ChunkManager() {}

	void InitChunks(string DataConf, string ModelPath, float scale) {
		int xMin = 0;
		int xMax = 2;
		int yMin = 0;
		int yMax = 2;

		for (int x = xMin; x < xMax; x++) {
			for (int y = yMin; y < yMax; y++) {
				std::cout << "[ChunkManager] Loading Chunk (" << std::to_string(x) << ";" << std::to_string(y) << ")!" << std::endl;

				//AnimModel chunkModel((ModelPath + "Chunk_" + std::to_string(x) + "x" + std::to_string(y) + ".dae"), glm::vec3(0, 0, 0), 0.2f);
				//Chunks.insert(make_pair(to_string(vec2(x, y)), Chunk{ chunkModel }));
			}
		}
	}

	Chunk GetChunk(vec3 position) {
		if (Chunks.count(to_string(WorldCordsToChunkCords(position))) == 1)
			return Chunks.at(to_string(WorldCordsToChunkCords(position)));
		else {
			std::cout << "Chunk at (" << WorldCordsToChunkCords(position).x << ";" << WorldCordsToChunkCords(position).y << ") not found" << std::endl;
			return Chunk{};
		}//*/
	}
	vector<Chunk> GetChunks() {
		vector<Chunk> localChunks;
		for (std::map<string, Chunk>::iterator it = Chunks.begin(); it != Chunks.end(); ++it) {
			localChunks.push_back(it->second);
		}
		return localChunks;//*/ 
	}

	float GetHeight(glm::vec3 PlayerPosition) {

		Chunk activeChunk = GetChunk(PlayerPosition); //Get Chunk where the player i currently in
		vector<Mesh*> activeMeshes = activeChunk.model.GetMeshes();
		vector<vec3> vertices = activeMeshes[activeMeshes.size() - 1]->GetTranslatedVertices(); //Get all vec3's of the chunk the player is in

		//Get 4 closes vertices to player position

		vec3 vecN0, vecN1, vecN2, vecN3;

		float line12x, line03x = PlayerPosition.x;
		float line12y, line03y = 0;

		calculateLineVars(vecN1, vecN2, line12x, line12y);
		calculateLineVars(vecN0, vecN3, line03x, line03y);

		float PlayerY = PlayerPosition.y;
		float PlayerZ = PlayerPosition.z;

		calculateLineVars(vec3(0, line12x, line12y), vec3(0, line03x, line03y), PlayerZ, PlayerY, true);
		return PlayerY;

	}

private:

	map<string, Chunk> Chunks;

	const int CHUNK_SIZE = 16;

	void calculateLineVars(vec3 point1, vec3 point2, float& x, float& y, bool useZ = false, bool calcX = false) {

		if (useZ && !calcX) {
			float a = (point2.y - point1.y) / (point2.z - point1.z);
			float b = point2.y - (a * point2.z);

			y = a * x + b;
		}
		else if(!calcX){
			float a = (point2.y - point1.y) / (point2.x - point1.x);
			float b = point2.y - (a * point2.x);

			y = a * x + b;
		}
		else if (useZ && calcX) {

		}
		else if (calcX) {

		}
	}

	std::vector<std::string> split(std::string s, char seperator)
	{
		std::vector<std::string> strings;
		istringstream f(s);
		string s2;
		while (getline(f, s2, seperator)) {
			strings.push_back(s2);
		}

		return strings;
	}
	vec2 WorldCordsToChunkCords(vec3 worldPos) {
		vec2 chunkCords = vec2((worldPos.x / CHUNK_SIZE), (worldPos.z / CHUNK_SIZE));
		return  chunkCords;
	}
};