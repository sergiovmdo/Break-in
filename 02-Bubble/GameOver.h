#pragma once
#include "TileMap.h"

class GameOver
{
public:
	GameOver();
	~GameOver();

	void init(int level);
	void update(int deltaTime);
	void render();
	void enterPressed();
	int getNextScene();

private:
	void initShaders();

	TileMap *retry, *menu;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int nextScene;
	int selectedOption;
	int optionSelectedTime = -1;
	bool enterPress;
	bool initialized = false;
};

