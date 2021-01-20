#ifndef MENU_INCLUDE
#define MENU_INCLUDE


#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Audio.h"
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 360


// MenuScene contains all the entities of our game.
// It is responsible for updating and render them.

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	void enterPressed();
	void charPressed(int ch);
	void removeChar();
	int getNextScene();

private:
	void initShaders();
	void checkPassword();

private:
	TileMap *mapPlay, *mapPassword, *mapCredits, *mapExit, *mapInstructions, *mapPasswordField, *mapPasswordBack;
	ShaderProgram texProgram, program;
	float currentTime;
	glm::mat4 projection;
	int nextScene = 0;
	int selectedOption;
	int optionSelectedTime = -1;
	bool enterPress;
	Text text;
	bool passwordMode;
	string password;
	bool audioInit = false;
	bool initialized = false;

};


#endif // MENU_INCLUDE

