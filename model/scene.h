#pragma once
#include "../config.h"
#include "cube.h"
#include "player.h"


class Scene {
public:
	Scene();
	~Scene();
	void update(float rate);
	void movePlayer(glm::vec3 dPos);
	void spinPlayer(glm::vec3 dEulers);
	void CreateACube(std::vector<Cube*> &cubes, int Total);
	void CreateASingleCube(std::vector<Cube*>& cubes, std::array<int, 3> CubeData);


	Cube* cube;
	Cube* cube2;
	Cube* cube3;
	Player* player;
};