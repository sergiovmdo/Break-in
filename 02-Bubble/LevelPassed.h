#pragma once
#include "Text.h"
#include "TileMap.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 360
#define MIN_TIME_BETWEEN_CHANGE_OPTIONS 200

class LevelPassed
{
public:
	LevelPassed();
	~LevelPassed();

	void init(int level);
	void update(int deltaTime);
	void render();
	void enterPressed();
	int getNextScene();

private:
	void initShaders();

	TileMap *next, *menu, *onlyMenu;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int nextScene;
	int scene;
	int selectedOption;
	int optionSelectedTime = -1;
	bool enterPress;
	Text password;
	bool initialized = false;
	string psw = "";
};

