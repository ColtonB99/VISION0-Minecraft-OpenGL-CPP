#include "engine.h"
#include <iostream>
#include <../model/cube.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <../World/Chunk.h>
#include <cmath>
#include <thread>
//#include <../view/rectangle_model.h>


//#include "../Math/NoiseGenerator.h"


// Define Variables outside of
// Function in Engine.cpp
// So all functions have access to them

const int total = 256; // Total Number of Blocks


std::vector<Chunk> chunks;
ChunkCreateInfo ChunkInfo;




// Cube* cubes[total]; // Cube array to hold the cubes
//RectangleModel* cubeModels[total]; // Rectangle array to hold the rectangles
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

Engine::Engine(int width, int height) {

	RectangleModelCreateInfo currentCube;
	currentCube.size = { 1.0f, 1.0f, 1.0f };
	RectangleModel* rectangleModel = new RectangleModel(&currentCube);
	


	shader = util::load_shader("shaders/vertex.txt", "shaders/fragment.txt"); // Creates / Loads the shader using the C Code
	glUseProgram(shader); // Installs the program object (Shader) as part of current rendering state.

	// glUniform1i means you're setting 1 integer within your uniform
	glUniform1i(glGetUniformLocation(shader, "basicTexture"), 0); // Tell the shader this texture has texture unit 0

	float aspectRatio = (float)width / float(height); // Set the aspect ratio (simple)
	//set up framebuffer
	// glClearColor(0.5f, 0.1f, 0.3f, 1.0f); // Sets the background color
	glClearColor(0.4f, 0.6f, 2.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK_NORMALS_HINT_PGI);


	glm::mat4 projection_transform = glm::perspective(-80.0f, aspectRatio, 0.1f, 100.0f); // 100.0f is the render distance
	glUniformMatrix4fv(
		glGetUniformLocation(shader, "projection"),
		1, GL_FALSE, glm::value_ptr(projection_transform)
	);

	scene = new Scene();
	createMaterials();



	



}


Engine::~Engine() {
	// Materials
	delete dirtMaterial;
	delete woodMaterial;
	delete stoneMaterial;
	delete grassMaterial;

	// Shader
	glDeleteProgram(shader);
}

void Engine::createMaterials() {
	MaterialCreateInfo woodMaterialInfo;
	woodMaterialInfo.filename = "textures/wood-low.jpg";
	woodMaterial = new Material(&woodMaterialInfo);

	// There's no way this is done incorrectly I think (But it's also new)
	MaterialCreateInfo dirtMaterialInfo;
	dirtMaterialInfo.filename = "textures/dirt.png";
	dirtMaterial = new Material(&dirtMaterialInfo);

	// There's no way this is done incorrectly I think (But it's also new)
	MaterialCreateInfo stoneMaterialInfo;
	stoneMaterialInfo.filename = "textures/stone.png";
	stoneMaterial = new Material(&stoneMaterialInfo);

	// There's no way this is done incorrectly I think (But it's also new)
	MaterialCreateInfo grassMaterialInfo;
	grassMaterialInfo.filename = "textures/visual-grass.jpg";
	grassMaterial = new Material(&grassMaterialInfo);
}

void Engine::render(Scene* scene) {


	glm::mat4 view_transform{
		glm::lookAt(
			scene->player->position,
			scene->player->position + scene->player->forwards,
			scene->player->up
		)
	};
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE,
		glm::value_ptr(view_transform));
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (Chunk* chunk : chunks)
	{
		for (Cube* cube : chunk->cubesy) {
			glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(cube->model_transform));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
	}

	

}


static std::thread* ChunkThread = nullptr;





void Engine::ChunkManager(Player* player, int RenderDistance) {
	std::vector<std::thread> ChunkThreads;
	// Get players current chunk position
	int plrChunkX = (int)player->position.x / 16;
	int plrChunkY = (int)player->position.y / 16;
	std::array<int, 2> currentPosition{ plrChunkX, plrChunkY };

	for (int x = RenderDistance * -1; x < RenderDistance; x++)
	{
		for (int y = RenderDistance * -1; y < RenderDistance; y++)
		{
			std::array<int, 2> curLocation = { plrChunkX + x, plrChunkY + y };
			CreateDeleteChunks(currentPosition, this->LastPosition, plrChunkX, plrChunkY, RenderDistance, curLocation);
		}
	}
}


Chunk* Engine::getChunk(glm::vec3 playerPosition) {

	

	std::array<int, 2> currentPosition{ (int)playerPosition.x / 16, (int)playerPosition.y / 16 };

	for (Chunk* chunk : chunks) {

		if ((int)playerPosition.x < 0 && (int)playerPosition.y < 0)
		{
			currentPosition = { ((int)playerPosition.x / 16) - 1, ((int)playerPosition.y / 16) - 1 };
			if (currentPosition == chunk->position)
			{
				return chunk;
			}
		}
		else if ((int)playerPosition.x < 0 && (int)playerPosition.y > -1)
		{
			currentPosition[0] = { ((int)playerPosition.x / 16) - 1 };
			if (currentPosition == chunk->position)
			{
				return chunk;
			}
		
		}
		else if ((int)playerPosition.x > -1 && (int)playerPosition.y < 0) {
			currentPosition[1] = { ((int)playerPosition.y / 16) - 1 };
			//currentPosition[1] = currentPosition[1] - 1;
			if (currentPosition == chunk->position)
			{
				return chunk;
			}
		}
		else 
		{
			if (currentPosition == chunk->position) {
				return chunk;
			}
		}

	}

}



int totalCalls = 0;
void Engine::KillChunk(int plrChunkX, int plrChunkY, int RenderDistance) {

	std::vector<Chunk*> chunkEraseList;
	for (Chunk* chunk : chunks) {		
		if (sqrt(pow(chunk->position[0] - plrChunkX, 2) + pow((chunk->position[1] - plrChunkY), 2)) > RenderDistance) {
			chunkEraseList.emplace_back(chunk);	
		}
	}

	for (Chunk* chunk : chunkEraseList) {
		chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
	}
}

void Engine::FreeChunk(std::array<int, 2> curLocation) {

	auto FreeChunk = std::find_if(chunks.begin(), chunks.end(), [curLocation](Chunk* curChunk) {return curChunk->position == curLocation; });
	if (FreeChunk == chunks.end())
	{
		
		ChunkCreateInfo m_chunkInfo;
		m_chunkInfo.position = curLocation;
		m_chunkInfo.Materials = {grassMaterial, woodMaterial};
		m_chunkInfo.shader = shader;
		chunks.push_back(new Chunk(&m_chunkInfo));
	}

}


void Engine::CreateDeleteChunks(std::array<int, 2> currentPosition, std::array<int, 2> lastPosition, int plrChunkX, int plrChunkY, int RenderDistance, std::array<int, 2> curLocation) {

	FreeChunk(curLocation);

	if (LastPosition != currentPosition)
	{
		LastPosition = currentPosition;
		KillChunk(plrChunkX, plrChunkY, RenderDistance);
		//ChunkThread = new std::thread([this, plrChunkX, plrChunkY, RenderDistance] {KillChunk(plrChunkX, plrChunkY, RenderDistance); });
		//ChunkThread->join();
	}
}