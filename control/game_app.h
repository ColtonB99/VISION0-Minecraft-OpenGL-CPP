#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "../view/engine.h"
#include <thread>
#include <future>

struct GameAppCreateInfo {
	int width;
	int height;
};

enum class returnCode {
	CONTINUE, QUIT
};

class GameApp {
public:
	GameApp(GameAppCreateInfo* createInfo);
	returnCode mainLoop();
	//glm::vec3 RayCast(Cube* cubes);
	~GameApp();
	ChunkCreateInfo* chunkInfo;
	
	

private:

	bool isGrounded;

	GLFWwindow* makeWindow();
	returnCode processInput();
	void calculateFrameRate();
	//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	int Colliding(glm::vec3 playerPosition);
	int CollideState;

	GLFWwindow* window;
	int width, height;
	Scene* scene;
	Engine* renderer;
	Chunk* chunker;
	

	double lastTime, currentTime;
	int numFrames;
	float frameTime;
	double currentTimea;
};