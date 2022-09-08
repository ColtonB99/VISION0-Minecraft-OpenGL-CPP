#include "config.h"
#include "control/game_app.h"

int main() {
	// 640, 480
	// 1280, 720
	int width = 1280;
	int height = 720;

	GameAppCreateInfo appInfo;
	appInfo.width = width;
	appInfo.height = height;
	GameApp* app = new GameApp(&appInfo);

	returnCode nextAction = returnCode::CONTINUE;
	while (nextAction == returnCode::CONTINUE) {
		nextAction = app->mainLoop();
	}

	delete app;

	return 0;
}