#pragma once
#include "Scenes.h"
#include "TileMap.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 360

class Instructions
{
public:
	Instructions();
	~Instructions();

	void init();
	void update(int deltaTime);
	void render();
	void enterPressed();
	int getNextScene();

private:
	void initShaders();

	TileMap *credits;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int nextScene = INSTRUCTIONS_SCENE;
	bool enterPress = false;
};

