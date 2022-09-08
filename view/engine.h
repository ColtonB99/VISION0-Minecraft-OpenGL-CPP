#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "shader.h"
#include "rectangle_model.h"
#include "material.h"
#include "../model/cube.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include "../World/Chunk.h"
#include <future>

class Engine {
public:
	Engine(int width, int height);
	~Engine();

	void createMaterials();
	void render(Scene* scene);
	void ChunkManager(Player* player, int RenderDistance);
	
	void FreeChunk(std::array<int, 2> curLocation);
	void KillChunk(int plrChunkX, int plrChunkY, int RenderDistance);
	Chunk* getChunk(glm::vec3 playerPosition);

	void CreateDeleteChunks(std::array<int, 2> currentPosition, std::array<int, 2> lastPosition, int plrChunkX, int plrChunkY, int RenderDistance, std::array<int, 2> curLocation);


	std::array<int, 2> LastPosition;
	unsigned int shader;
	Material* woodMaterial;
	Material* dirtMaterial;
	Material* stoneMaterial;
	Material* grassMaterial;

	RectangleModelCreateInfo currentCube;

	RectangleModel* rectangleModel;

	std::vector<std::future<void>> m_Futures;


	Chunk* chunker;
	
	Scene* scene;
	std::vector<Chunk*> chunks;
	GLFWmousebuttonfun mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};