#include "cube.h"
#include <iostream>
#include "../control/game_app.h"

//GameApp gameapp;

Cube::Cube(CubeCreateInfo* createInfo) {
	this->position = createInfo->position;
	this->eulers = createInfo->eulers;
	this->material = createInfo->material;
	this->model_transform = { glm::mat4(1.0f) };

	//glm::mat4 model_transform{ glm::mat4(1.0f) };
	this->model_transform = glm::translate(model_transform, this->position);



	//glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(this->model_transform));
	

}

//void Cube::remove() {
//	delete this;
//}

void Cube::update(float rate) {

}

//Cube::~Cube() {
//	delete this;
//}