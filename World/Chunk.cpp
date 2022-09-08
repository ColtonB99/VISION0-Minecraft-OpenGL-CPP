#include "Chunk.h"
#include <iostream>




Chunk::Chunk(ChunkCreateInfo* chunkInfo) {
	this->position = { chunkInfo->position[0], chunkInfo->position[1] };
	this->grassMaterial = chunkInfo->grassMaterial;
	this->shader = chunkInfo->shader;

	/*MaterialCreateInfo grassMaterialInfo;
	grassMaterialInfo.filename = "textures/visual-grass.jpg";
	grassMaterial = new Material(&grassMaterialInfo);*/


	bool doneMoreThanOnce = false;

	int ChunkPositionX = chunkInfo->position[0];
	int ChunkPositionY = chunkInfo->position[1];

	//std::cout << "Create Chunk Was Called" << std::endl;
	std::vector<int> ChunkSize{ 16, 14, 16 };
	std::vector<int> NoiseScale{ 1, 1 };

	// Reset below to NoiseOffset{0,0}; to fix any errors
	std::vector<int> NoiseOffset{ ChunkPositionX, ChunkPositionY };
	int HeightOffset = 8;
	float HeightIntensity = 5.0f;
	//NoiseGenerator PerlinNoise = new NoiseGenerator;
	const siv::PerlinNoise::seed_type seed = 14786u;
	const siv::PerlinNoise perlin{ seed };

	for (int x = 0; x < ChunkSize[0]; x++)
	{
		for (int z = 0; z < ChunkSize[2]; z++)
		{
			// X = 0, Y = 1, Z = 2
			float PerlinCoordX = NoiseOffset[0] + x / (float)ChunkSize[0] * NoiseScale[0];
			float PerlinCoordY = NoiseOffset[1] + z / (float)ChunkSize[2] * NoiseScale[1];
			int HeightGen = std::round(perlin.noise2D((double)PerlinCoordX, (double)PerlinCoordY) * HeightIntensity + HeightOffset);
			//std::cout << HeightGen << std::endl;
			//std::cout << HeightGen << std::endl;

			for (int y = HeightGen; y >= 0; y--)
			{
				int BlockTypeToAssign = 0;

				// Set the first layer to grass
				if (y == HeightGen) BlockTypeToAssign = 0; grassMaterial->use();

				// Set the next 3 layers to dirt
				if (y < HeightGen && y > HeightGen - 4) BlockTypeToAssign = 1;



				// Set everything between the dirt range (inclusive) and 0 (exclusive) to stone
				if (y < HeightGen - 4 && y > 0) BlockTypeToAssign = 2;

				// Set everything at height 0 to bedrock
				if (y == 0) BlockTypeToAssign = 3;

				std::array<float, 3> LoopData{ (float)x, (float)y, (float)z };


				//if (y == HeightGen) std::array<int, 3> LoopData{ x, y, z + 50 };;


				// Only draw top layer
				
				if (y == HeightGen) 
				{
					
					CubeCreateInfo cubesInfo;
					cubesInfo.position = { LoopData[0] + (ChunkPositionX * 16), LoopData[2] + (ChunkPositionY * 16), LoopData[1] };
					cubesInfo.material = BlockTypeToAssign;
					Cube* a = new Cube(&cubesInfo);
					this->cubesy.push_back(a);

									

					


					


				}
			}
		}
	}
}


bool Chunk::CheckCorners(const siv::PerlinNoise perlin, int ChunkPositionX, int ChunkPositionY) {
	std::vector<int> ChunkSize{ 16, 14, 16 };
	std::vector<int> NoiseScale{ 1, 1 };
	std::vector<int> NoiseOffset{ ChunkPositionX, ChunkPositionY };

	// Check up
	float PerlinCoordX = NoiseOffset[0] - 16 / (float)ChunkSize[0] * NoiseScale[0];


	//
	//float PerlinCoordY = NoiseOffset[1] + z / (float)ChunkSize[2] * NoiseScale[1];
	return true;

}


void Chunk::RemoveChunk() {
	for (Cube* obj : this->cubesy) { delete obj; }
	//for (RectangleModel* obj : this->rectanglesy) { delete obj; }
	//delete this;
}


int Chunk::getBlock(glm::vec3 playerPosition) {

	
	float cubeSize = 1.0f;
	int cubeIndex = 0;
	//std::array<int, 2> playerChunkPosition{ (int)playerPosition.x / 16, (int)playerPosition.y / 16 };

	float playerPosX = playerPosition.x;
	float playerPosY = playerPosition.y;
	float playerPosZ = playerPosition.z;


	auto it = find_if(cubesy.begin(), cubesy.end(), [playerPosX, playerPosY, playerPosZ, cubeSize](Cube* cube) { return sqrt(pow((cube->position.x) - (playerPosX), 2) + pow((cube->position.y) - (playerPosY), 2) + pow((cube->position.z) - (playerPosZ), 2)) <= cubeSize; });
	if (it != cubesy.end()) {
		return 3105;
	}


		









	//for (Cube* cube : cubesy) {

	//	

	//		if () {
	//			//std::cout << "Collision Detected at:" << cubeIndex << std::endl;
	//			return 3105;
	//		}

	//	




	//	
	//	/*if (playerPosX <= cube->position.x + cubeSize && playerPosX >= cube->position.x - cubeSize) {
	//		if (playerPosY <= cube->position.y + cubeSize && playerPosY >= cube->position.y - cubeSize) {
	//			if (playerPosZ <= cube->position.z + cubeSize && playerPosZ >= cube->position.z - cubeSize) {
	//				std::cout << "Collision Detected at:" << cubeIndex << std::endl;
	//				return 3105;
	//			}
	//			else {
	//				return 1051;
	//			}
	//		}
	//	}*/
	//	/*cubeIndex++;*/
	//}

}