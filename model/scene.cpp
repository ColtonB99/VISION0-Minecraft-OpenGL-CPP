#include "scene.h"
#include <array>


Scene::Scene() {

	PlayerCreateInfo playerInfo;
	playerInfo.eulers = { 300.0f, 170.0f, 90.0f };
	playerInfo.position = { 0.0f, 0.0f, 0.0f };
	player = new Player(&playerInfo);


};

void Scene::CreateACube(std::vector<Cube*> &cubes, int Total) {
	
	CubeCreateInfo cubesInfo;
	cubesInfo.eulers = { 0.0f, 0.0f, 0.0f };
	
	int TotalSqrt = sqrt(Total); // I use the sqaure root to reset 0 position
	// So it builds a flat x by x plane rather than a line
	// This was done by trial and error so theres bound to be an inefficiency somewhere
	// believe me when I say I have no idea what i'm doing

	int liner;
	for (int i = 0; i < Total; i++)
	{
		liner = std::floor(i / TotalSqrt);
		cubesInfo.position = {
		0.0f + i - (liner * TotalSqrt),
		0.0f + (liner), 
		-2.0f };

		cubes.push_back(new Cube(&cubesInfo));
	}
}

void Scene::CreateASingleCube(std::vector<Cube*>&cubeso, std::array<int,3> CubeData) {
	CubeCreateInfo cubesInfo;
	cubesInfo.eulers = { 0.0f, 0.0f, 0.0f };
	cubesInfo.position = { CubeData[0], CubeData[1], CubeData[2] };
	cubeso.push_back(new Cube(&cubesInfo));

}



Scene::~Scene() {
	delete player;
}




void Scene::update(float rate) {
	player->update();

	


	//std::cout << player->position.x << ", " << player->position.y << ", " << player->position.z << std::endl; Accurately represents players position in space
	//std::cout << "In chunk: X=" << (int)player->position.x / 16 << ", Y=" << (int)player->position.y / 16 << std::endl; // Accurately represents what chunk the player is in

	//renderer->ChunkManager(player->position.x, player->position.y);

}

void Scene::movePlayer(glm::vec3 dPos) {
	
	player->position += dPos;
}

void Scene::spinPlayer(glm::vec3 dEulers) {
	player->eulers += dEulers;

	if (player->eulers.z < 0) {
		player->eulers.z += 360;
	}
	else if (player->eulers.z > 360) {
		player->eulers.z -= 360;
	}

	player->eulers.y = std::max(std::min(player->eulers.y, 179.0f), 1.0f);
}