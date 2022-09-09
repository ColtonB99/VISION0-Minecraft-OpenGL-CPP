#include "game_app.h"
#include "../Math/Ray.h"
#include "../World/Chunk.h"
#include <thread>


GameApp::GameApp(GameAppCreateInfo* createInfo) {
	this->width = createInfo->width;
	this->height = createInfo->height;

	//void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
	
	lastTime = glfwGetTime();
	numFrames = 0;
	frameTime = 16.0f;
	window = makeWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	renderer = new Engine(width, height);
	scene = new Scene();



	//MaterialCreateInfo woodMaterialInfo;
	//woodMaterialInfo.filename = "textures/wood-low.jpg";
	//woodMaterial = new Material(&woodMaterialInfo);


	////Material* woodMaterial = renderer->woodMaterial;
	scene->player->position.x = 0;
	scene->player->position.y = 0;
	scene->player->position.z = 15;


	std::cout << "called" << std::endl;
	
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	
}

//void GameApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//		//deleteCube(cubes, 22);
//		std::cout << "yoo \n";
//
//
//	}
//
//	/*if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
//		std::cout << "Left click Release" << std::endl;
//	}*/
//
//}


GLFWwindow* GameApp::makeWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "This is working I hope", NULL, NULL); // Change first NULL to "glfwGetPrimaryMonitor()" for fullscreen

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD initialization failed\n";
		return NULL;
	}
	
	glViewport(0, 0, width, height);

	

	return window;
}

int flipper = 0;

returnCode GameApp::processInput() {

	int wasdState{ 0 };
	int spaceState{ 0 };
	float walk_direction{ scene->player->eulers.z };
	float jump_direction{ scene->player->position.z };
	bool walking{ false };
	bool jumping{ false };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		wasdState += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		wasdState += 2;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		wasdState += 4;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		wasdState += 8;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		spaceState += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		spaceState += 2;
	}

	switch (wasdState) {
	case 1:
	case 11:
		//forwards
		walking = true;
		break;
	case 3:
		//left-forwards
		walking = true;
		walk_direction += 45;
		break;
	case 2:
	case 7:
		//left
		walking = true;
		walk_direction += 90;
		break;
	case 6:
		//left-backwards
		walking = true;
		walk_direction += 135;
		break;
	case 4:
	case 14:
		//backwards
		walking = true;
		walk_direction += 180;
		break;
	case 12:
		//right-backwards
		walking = true;
		walk_direction += 225;
		break;
	case 8:
	case 13:
		//right
		walking = true;
		walk_direction += 270;
		break;
	case 9:
		//right-forwards
		walking = true;
		walk_direction += 315;
	}

	switch (spaceState) {
	case 1:
		jumping = true;
		jump_direction += 180;
		break;
	case 2:
		jumping = true;
		jump_direction -= 180;
		break;
	}
	
	float movingSpeed = 0.2f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		movingSpeed *= 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		movingSpeed /= 2;
	}
	
	glm::vec3 dPos = scene->player->position + movingSpeed * frameTime / 16.0f * glm::vec3{ glm::cos(glm::radians(walk_direction)),	glm::sin(glm::radians(walk_direction)),	0.0f };

	


	if (walking) {
		glm::vec3 walkToX = movingSpeed * frameTime / 16.0f * glm::vec3{ glm::cos(glm::radians(walk_direction)), 0.0f, 0.0f };
		glm::vec3 PlayerPos = scene->player->position;
		PlayerPos.z -= .5f;
		if (Colliding(PlayerPos += walkToX) != 3105) {
			scene->movePlayer(
				movingSpeed * frameTime / 16.0f * glm::vec3{
					glm::cos(glm::radians(walk_direction)),
					0.0f,
					0.0f
				}
			);
		}
		glm::vec3 walkToY = movingSpeed * frameTime / 16.0f * glm::vec3{ 0.0f, glm::sin(glm::radians(walk_direction)), 0.0f };

		if (Colliding(PlayerPos += walkToY) != 3105) {
			scene->movePlayer(walkToY);
		}




		
	} 

	glm::vec3 jPos = scene->player->position + 0.1f * frameTime / 16.0f * glm::vec3{ 0.0f, 0.0f, jump_direction * 0.007f };

	if (jumping) {
		scene->movePlayer(
			0.1f * frameTime / 16.0f * glm::vec3{
					0.0f, 0.0f, jump_direction * 0.005f
			}
		);
	}
	else {
		glm::vec3 PlayerPos = scene->player->position;
		PlayerPos.z -= 1.0f;

		glm::vec3 MoveTo = 0.1f * frameTime / 16.0f * glm::vec3{ 0.0f, 0.0f, -0.20f };

		if (Colliding(PlayerPos += MoveTo) != 3105) {
			scene->movePlayer(
				0.1f * frameTime / 16.0f * glm::vec3{
						0.0f, 0.0f, -0.80f
				}
			);
		}
	}

	
	

	

	float mouseSensitivity = 0.3f;
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	glfwSetCursorPos(window, static_cast<double>(width / 2), static_cast<double>(height / 2));

	float delta_x{ static_cast<float>(mouse_x - static_cast<double>(width / 2)) };
	float delta_y{ static_cast<float>(mouse_y - static_cast<double>(height / 2)) };
	scene->spinPlayer(
		frameTime / 16.0f * mouseSensitivity * glm::vec3 {
			0.0f, delta_y, -delta_x
			
		}
		
	);


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		if (flipper == 0) {
			flipper = 1;


			for (Ray ray({ scene->player->position.x, scene->player->position.y + 0.06f,
				scene->player->position.z },
				scene->player->eulers);
				ray.getLength() < 6; ray.step(0.05f)) {
				int x = static_cast<int>(ray.getEnd().x);
				int y = static_cast<int>(ray.getEnd().y);
				int z = static_cast<int>(ray.getEnd().z);

				//auto block = renderer->cubes[22]; // EDIT THIS LATER, THIS IS WHERE WE FIND THE BLOCK

				//renderer->deleteCube(renderer->cubes, 22);
			}
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		flipper = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return returnCode::QUIT;
	}
	return returnCode::CONTINUE;
}

std::array<int, 2> LocationThen;
returnCode GameApp::mainLoop() {

	


	
	glfwPollEvents();

	//update
	scene->update(frameTime / 16.0f);

	// Draw

	renderer->render(scene);

	std::array<int, 2> LocationNow = { (int)(scene->player->position.x / 16) , (int)(scene->player->position.y / 16) };
	/*
	if (LocationNow != LocationThen) {
		LocationThen = LocationNow;
		std::cout << LocationNow[0] << ", " << LocationNow[1] << std::endl;
	}*/
	

	


	// Manage Chunks
	renderer->ChunkManager(scene->player, 6);
	returnCode nextAction{ processInput() };
	

	//CollidingNow = renderer->getChunk(scene->player->position)->getBlock(scene->player->position);

	
	//glfwSwapBuffers(window);

	glFlush();

	calculateFrameRate();
	
	return nextAction;
}

GameApp::~GameApp() {
	//free memory
	delete scene;
	delete renderer;
	glfwTerminate();
}


int GameApp::Colliding(glm::vec3 playerPosition) {

	return renderer->getChunk(playerPosition)->getBlock(playerPosition);

}


void GameApp::calculateFrameRate() {
	currentTime = glfwGetTime();
	double delta = currentTime - lastTime;

	if (delta >= 1) {
		int framerate{ std::max(1, int(numFrames / delta)) };
		std::stringstream title;
		title << "Running at " << framerate << " fps.";
		glfwSetWindowTitle(window, title.str().c_str());
		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(1000.0 / framerate);
	}

	++numFrames;
}

//glm::vec3 RayCast(Cube* cubes) {
//	//float jump_direction{ scene->player->position.z };
//	return 0;
//}