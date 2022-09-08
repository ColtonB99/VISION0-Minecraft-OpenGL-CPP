#pragma once
#include "../config.h"
#include "material.h"

struct RectangleModelCreateInfo {
	glm::vec3 size, position;
};

class RectangleModel {
public:
	unsigned int VBO, VAO, vertexCount;
	std::vector<float> vertices;
	Material* woodMaterial;
	RectangleModel(RectangleModelCreateInfo* createInfo);
	/*~RectangleModel();*/
};