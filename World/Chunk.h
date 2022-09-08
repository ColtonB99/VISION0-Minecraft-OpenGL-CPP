#pragma once
#include "../config.h"
#include <vector>
#include "WorldConstants.h"
#include "../view/rectangle_model.h"
#include "../model/cube.h"
#include <array>
#include "../Math/PerlinNoise.hpp"
#include "../view/material.h"

struct ChunkCreateInfo {
	std::array<int, 2> position;
	Material* grassMaterial;
	unsigned int shader;


};




class Chunk {
public:
	bool CheckCorners(const siv::PerlinNoise perlin, int ChunkPositionX, int ChunkPositionY);
	std::array<int, 2> position;
	std::vector<Cube*> cubesy;
	
	Material* grassMaterial;
	unsigned int shader;
	

	int getBlock(glm::vec3 playerPosition);
	Chunk(ChunkCreateInfo* chunkInfo);
	void RemoveChunk();
	Chunk* CreateChunk(int ChunkPositionX, int ChunkPositionY);
	
};

