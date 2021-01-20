#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Audio.h"

#include <map>


#include "Game.h"
#include "Password.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 360
#define MIN_TIME_BETWEEN_CHANGE_OPTIONS 200


Menu::Menu()
{
	nextScene = 0;
	enterPress = false;
}


Menu::~Menu()
{
	if (mapPlay != NULL)
	{
		delete mapPlay;
	}

	if (mapPassword != NULL)
	{
		delete mapPassword;
	}

	if (mapCredits != NULL)
	{
		delete mapCredits;
	}

	if (mapExit != NULL)
	{
		delete mapExit;
	}

	if (mapInstructions != NULL)
	{
		delete mapInstructions;
	}

	if (mapPasswordBack != NULL)
	{
		delete mapPasswordBack;
	}

	if (mapPasswordField != NULL)
	{
		delete mapPasswordField;
	}
}

void Menu::init()
{
	nextScene = MENU_SCENE;
	passwordMode = false;
	password = "";

	Audio::instance().playMenu();


	if (initialized)
	{
		return;
	}

	initialized = true;
	initShaders();
	mapPlay = TileMap::createTileMapMenu("levels/menu_play.txt", glm::vec2(0, 0), texProgram);
	mapPassword = TileMap::createTileMapMenu("levels/menu_password.txt", glm::vec2(0, 0), texProgram);
	mapCredits = TileMap::createTileMapMenu("levels/menu_credits.txt", glm::vec2(0, 0), texProgram);
	mapExit = TileMap::createTileMapMenu("levels/menu_exit.txt", glm::vec2(0, 0), texProgram);
	mapInstructions = TileMap::createTileMapMenu("levels/menu_instructions.txt", glm::vec2(0, 0), texProgram);
	mapPasswordBack = TileMap::createTileMapMenu("levels/menu_password_back.txt", glm::vec2(0, 0), texProgram);
	mapPasswordField = TileMap::createTileMapMenu("levels/menu_password_field.txt", glm::vec2(0, 0), texProgram);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!text.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");

	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
}

void Menu::render()
{

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	auto modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	switch (selectedOption)
	{
	default:
	case 0:
		if (passwordMode) {
			mapPasswordField->render();
		}
		else {
			mapPlay->render();
		}
		break;
	case 1:
		if (passwordMode)
		{
			mapPasswordBack->render();
		}
		else {
			mapPassword->render();
		}
		break;
	case 2:
		mapInstructions->render();
		break;
	case 3:
		mapCredits->render();
		break;
	case 4:
		mapExit->render();
		break;
	}

	if (passwordMode)
	{
		text.render(password, glm::vec2(CAMERA_WIDTH / 3, CAMERA_HEIGHT / 2 + 10), 20, glm::vec4(1, 1, 1, 1));
	}

}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	bool play = true;
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (currentTime - optionSelectedTime >= MIN_TIME_BETWEEN_CHANGE_OPTIONS) {
			if (passwordMode)
			{
				if (selectedOption == 1)
				{
					selectedOption = 0;
				}
				else
				{
					selectedOption++;
				}
			}
			else {
				if (selectedOption == 4)
				{
					selectedOption = 0;
				}
				else
				{
					selectedOption++;
				}
			}
			optionSelectedTime = currentTime;
			Audio::instance().play(MENU_CLICK, false);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (currentTime - optionSelectedTime >= MIN_TIME_BETWEEN_CHANGE_OPTIONS) {
			if (passwordMode)
			{
				if (selectedOption == 0)
				{
					selectedOption = 1;
				}
				else
				{
					selectedOption--;
				}
			}
			else {
				if (selectedOption == 0)
				{
					selectedOption = 4;
				}
				else
				{
					selectedOption--;
				}
			}
			optionSelectedTime = currentTime;
			Audio::instance().play(MENU_CLICK, false);
		}
	}

	if (enterPress)
	{
		enterPress = false;
		Audio::instance().play(MENU_CLICK, false);

		if (passwordMode)
		{
			if (selectedOption == 1) {
				passwordMode = false;
				selectedOption = 1;
			}
			else
			{
				checkPassword();
			}
			return;
		}

		switch (selectedOption)
		{
		case 0:
			nextScene = GAME_SCENE_1;
			break;
		case 1:
			nextScene = MENU_SCENE;
			passwordMode = true;
			password = "";
			selectedOption = 0;
			break;
		case 2:
			nextScene = INSTRUCTIONS_SCENE;
			break;
		case 3:
			nextScene = CREDITS_SCENE;
			break;
		case 4:
			nextScene = EXIT_SCENE;
			break;
		default:
			nextScene = MENU_SCENE;
		}
	}
}

void Menu::charPressed(int ch)
{
	if (passwordMode && selectedOption == 0 && password.size() < 10)
	{
		password += char(ch);
	}
}

void Menu::removeChar()
{
	if (passwordMode && selectedOption == 0 && !password.empty())
	{
		password.pop_back();
	}
}


void Menu::enterPressed()
{
	enterPress = true;
}


int Menu::getNextScene()
{
	return nextScene;
}

void Menu::checkPassword()
{
	const auto lvl = Password::checkPassword(password);

	if (lvl != -1)
	{
		nextScene = lvl;
		selectedOption = 0;
	}
	else
	{
		Audio::instance().play(INCORRECT, false);
	}
}



