#pragma once
#include "../config.h"




struct CubeCreateInfo {
	glm::vec3 position, eulers;
	int material;
	glm::mat4 model_transform;
	
};

class Cube {
public:
	glm::vec3 position, eulers;
	int material;
	glm::mat4 model_transform;
	Cube(CubeCreateInfo* createInfo);
	void createMaterials();
	//void remove();
	void update(float rate);
	//~Cube();
};

