#pragma once
#include "../config.h"
//#include "../view/engine.h" DONT DO THIS, IT BREAKS IT FOR SOME REASON

struct PlayerCreateInfo {
	glm::vec3 position, eulers;
};

class Player {
public:
	glm::vec3 position, eulers, up, forwards, right;

	Player(PlayerCreateInfo* createInfo);
	void update();
};
