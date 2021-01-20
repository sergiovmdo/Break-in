#include "LevelPassed.h"

#include <iostream>
#include <GL/freeglut_std.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Audio.h"
#include "Game.h"
#include "Password.h"
#include "Scenes.h"


LevelPassed::LevelPassed()
{
	enterPress = false;
}


LevelPassed::~LevelPassed()
{
	if (next != NULL)
	{
		delete next;
	}

	if (menu != NULL)
	{
		delete menu;
	}

	if (onlyMenu != NULL)
	{
		delete onlyMenu;
	}
}

void LevelPassed::init(int level)
{
	scene = level;
	nextScene = level;
	Audio::instance().stopAll();

	if (level == LEVEL_PASSED_SCENE_3)
		Audio::instance().play(END_GAME, false);
	else
		Audio::instance().play(LEVEL_PASSED, false);
	switch (level)
	{
	case LEVEL_PASSED_SCENE_1:
		psw = "KEY IS " + PASSWORD_LEVEL_2;
		break;
	case LEVEL_PASSED_SCENE_2:
		psw = "KEY IS " + PASSWORD_LEVEL_3;
		break;
	case LEVEL_PASSED_SCENE_3:
		psw = "";
		break;
	}

	if (initialized)
	{
		return;
	}

	initialized = true;
	initShaders();

	next = TileMap::createTileMapMenu("levels/scenes/level_passed_next.txt", glm::vec2(0, 0), texProgram);
	menu = TileMap::createTileMapMenu("levels/scenes/level_passed_menu.txt", glm::vec2(0, 0), texProgram);
	onlyMenu = TileMap::createTileMapMenu("levels/scenes/game_passed.txt", glm::vec2(0, 0), texProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (scene != LEVEL_PASSED_SCENE_3)
	{
		if (!password.init("fonts/bo.ttf"))
			cout << "Could not load font!!!" << endl;
	}
	
}

void LevelPassed::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	auto modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (scene == LEVEL_PASSED_SCENE_3)
	{
		onlyMenu->render();
	}
	else {
		switch (selectedOption)
		{
		case 0:
			next->render();
			break;
		case 1:
			menu->render();
			break;
		}
	}

	if (scene != LEVEL_PASSED_SCENE_3)
	{
		password.render(psw, glm::vec2(255, 220), 11, glm::vec4(1, 1, 1, 1));
	}

	
}

void LevelPassed::update(int deltaTime)
{
	currentTime += deltaTime;
	bool play = true;
	if (scene != LEVEL_PASSED_SCENE_3)
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) || Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (currentTime - optionSelectedTime >= MIN_TIME_BETWEEN_CHANGE_OPTIONS) {
				if (selectedOption == 0)
				{
					selectedOption = 1;
				}
				else
				{
					selectedOption = 0;
				}
				optionSelectedTime = currentTime;
				Audio::instance().play(MENU_CLICK, false);
			}
		}
	}
	
	if (enterPress)
	{
		enterPress = false;
		Audio::instance().play(MENU_CLICK, false);
		switch (selectedOption)
		{
		case 0:
			switch (nextScene)
			{
			case LEVEL_PASSED_SCENE_1:
				nextScene = GAME_SCENE_2;
				break;
			case LEVEL_PASSED_SCENE_2:
				nextScene = GAME_SCENE_3;
				break;
			case LEVEL_PASSED_SCENE_3:
				nextScene = MENU_SCENE;
				break;
			}
			break;
		case 1:
			nextScene = MENU_SCENE;
		}
	}
}

void LevelPassed::enterPressed()
{
	enterPress = true;
}


int LevelPassed::getNextScene()
{
	return nextScene;
}



void LevelPassed::initShaders()
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

