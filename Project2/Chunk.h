#pragma once
#include <string>
#include "CImg.h"

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "CollisionExtra.h"
#include "Model.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"

using namespace cimg_library;

class Chunk
{
public:
	ComponentSystemManager csm;

	Chunk(Chunk &chunk) {
		std::cout << "Using Refrence Struct" << std::endl;
	}

	~Chunk() {}
	Chunk(BoundingBox _boundingBox, glm::vec3 position, const char *modelPath) {
		Model model(modelPath, position, 1.f);
		chunkModel.push_back(model);

		///BoundingBox _boundingBox{boundingBox->min, boundingBox->max, boundingBox->id};

		auto chunkEntity = csm.CreateEntity();
		csm.AddComponent(chunkEntity, RenderObjectC{ model });
		csm.AddComponent(chunkEntity, NonEntityC{});
		csm.AddComponent(chunkEntity, CollisionC{});
		csm.AddComponent(chunkEntity, BoundingBoxC{ _boundingBox, false, nullptr, 0 });
	}
	Chunk(glm::vec3 position, const char *modelPath) {
	}

	float GetHeight(glm::vec3 position) {
		Model model = chunkModel.at(chunkModel.size() - 1);
		float x = (int)position.x + .5f;
		float z = (int)position.z + .5f;
		float y = 0;
		float midY = 0;

		glm::vec3 FirstCornor, PrevCorner = glm::vec3(0, 0, 0);
		for (glm::vec3 corner : model.getCubeCorners(glm::vec2(x, z))) {
			//Loops 4 times (each corner once)
			//Distance between 2 corners is 1 (its a square of 1x1)
			if (FirstCornor == glm::vec3(0, 0, 0)) {
				FirstCornor = corner;
				break;
			}

			midY += FloatOnLine(PrevCorner, corner, position, (PrevCorner.z == corner.z));

			PrevCorner = corner;
		}
		midY += FloatOnLine(FirstCornor, PrevCorner, position, (FirstCornor.z == PrevCorner.z));

		y = midY / 4;

		return y;
	}

private:
	glm::vec3 Position;
	std::vector<Model> chunkModel;

	float FloatOnLine(glm::vec3 prevCorner, glm::vec3 corner, glm::vec2 position, bool UseZCoord = false) {
		float PositionOnLine = 0;

		if (!UseZCoord) PositionOnLine = (prevCorner.x + corner.x) / position.x;
		else if (UseZCoord) PositionOnLine = (prevCorner.z + corner.z) / position.y;

		return ((prevCorner.y + corner.y) / PositionOnLine);
	}
};

class ChunkManager {
public:
	Chunk GetChunk(BoundingBox _ChunkBoundingBox) {
		///BoundingBox* _ChunkBoundingBox = new BoundingBox{ ChunkBoundingBox };
		if (chunks.find(_ChunkBoundingBox) != chunks.end()) {
			return chunks.at(_ChunkBoundingBox);
		}
	}
	void AddChunk(BoundingBox ChunkBoundingBox, Chunk chunk) {
		if (chunks.find(ChunkBoundingBox) == chunks.end()) {
			chunks.insert_or_assign(ChunkBoundingBox, chunk);
		}
		else {
			//Throw error that chunk already exsists
		}
	}

	void InitializeChunks(const char *pathTxt, const char* pathImage) {
		CImg<unsigned char> src(pathImage);
		int width = src.width();
		int height = src.height();

		std::ifstream infile(pathTxt);
		std::string line = "";
		while (std::getline(infile, line)) {
			std::vector<std::string> data = ToolBox::split(line, '=');
			ModelPathForChunk.insert_or_assign(std::stoi(data.at(0)), data.at(data.size() - 1));
		}

		int CurrentChunkNum = 0;
		int ChunkId = 0;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; x++) {
				unsigned char* ptr = src.data(x, y); // get pointer to pixel @ x,y

				//ChunkId = ptr->ColorR (Get Used Color(s) from config.ini)
				ChunkId += (int)src.data(x, y, 0, 0);
				ChunkId += (int)src.data(x, y, 0, 1);
				ChunkId += (int)src.data(x, y, 0, 2);

				//Generate HitBox
				BoundingBox boundingBox = BoundingBox{ glm::vec3(x * 16, 0, x * 16), glm::vec3((x * 16) + 16, 256, (x * 16) + 16) };

				//Generate Position on Size & X & Y from loops
				glm::vec3 chunkPosition(x * 16, 0, x * 16);

				//Chunk newChunk(Position, res/Models/Chunks/Chunk_ChunkId);
				Chunk newChunk(boundingBox, chunkPosition, ModelPathForChunk.at(ChunkId).c_str());

				AddChunk(boundingBox, newChunk);

				CurrentChunkNum++;
			}
		}
	}

private:
	std::map<BoundingBox, Chunk> chunks;
	std::map<int, std::string> ModelPathForChunk;
};

