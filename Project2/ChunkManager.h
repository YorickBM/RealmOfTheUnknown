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
		std::cout << "[ChunkManager] Loading File..." << std::endl;
		string line;
		vector<string> data;
		ifstream myfile(DataConf);
		if (myfile.is_open())
		{
			std::cout << "------" << std::endl;
			while (getline(myfile, line))
			{
				std::cout << line << "\n";
				data.push_back(line);
			}
			myfile.close();
		}
		std::cout << "[ChunkManager] Loaded File..." << std::endl;

		int xMin = 0;
		int xMax = 0;
		int yMin = 0;
		int yMax = 0;

		for (std::string s : data) {
			vector<string> splited = split(s, '=');
			if (splited[0] == "MaxWidth") xMax = std::stoi(splited[2]);
			if (splited[0] == "MaxHeight") yMax = std::stoi(splited[2]);
			if (splited[0] == "MinWidth") xMin = std::stoi(splited[2]);
			if (splited[0] == "MinHeight") yMin = std::stoi(splited[2]);
		}

		for (int x = xMin; x < xMax; x++) {
			for (int y = yMin; y < yMax; y++) {
				std::cout << "[ChunkManager] Loading Chunk (" << std::to_string(x) << ";" << std::to_string(y) << ")!" << std::endl;

				AnimModel chunkModel((ModelPath + "Chunk_" + std::to_string(x) + "X" + std::to_string(y)), glm::vec3(x * 16, 0, y * 16), 1.0f);
				Chunks.insert(make_pair(to_string(vec2(x, y)), Chunk{ chunkModel }));
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

private:
	map<string, Chunk> Chunks;
	const int CHUNK_SIZE = 16;

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