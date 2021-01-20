#include "GameOver.h"



#include <iostream>
#include <GL/freeglut_std.h>
#include <glm/gtc/matrix_transform.hpp>


#include "Audio.h"
#include "Game.h"


GameOver::GameOver()
{
	enterPress = false;
}


GameOver::~GameOver()
{
	if (retry != NULL)
	{
		delete retry;
	}

	if (menu != NULL)
	{
		delete menu;
	}
}

void GameOver::init(int level)
{
	nextScene = level;
	Audio::instance().stopAll();
	Audio::instance().play(GAME_OVER, false);

	if (initialized)
	{
		return;
	}

	initialized = true;
	initShaders();

	retry = TileMap::createTileMapMenu("levels/scenes/game_over_retry.txt", glm::vec2(0, 0), texProgram);
	menu = TileMap::createTileMapMenu("levels/scenes/game_over_menu.txt", glm::vec2(0, 0), texProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void GameOver::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	auto modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	switch (selectedOption)
	{
	case 0:
		retry->render();
		break;
	case 1:
		menu->render();
		break;
	}

}

void GameOver::update(int deltaTime)
{
	currentTime += deltaTime;
	bool play = true;
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

	if (enterPress)
	{
		enterPress = false;
		Audio::instance().play(MENU_CLICK, false);
		switch (selectedOption)
		{
		case 0:
			switch (nextScene)
			{
			case GAME_OVER_SCENE_1:
				nextScene = GAME_SCENE_1;
				break;
			case GAME_OVER_SCENE_2:
				nextScene = GAME_SCENE_2;
				break;
			case GAME_OVER_SCENE_3:
				nextScene = GAME_SCENE_3;
				break;
			}
			break;
		case 1:
			nextScene = MENU_SCENE;
		}
	}
}

void GameOver::enterPressed()
{
	enterPress = true;
}


int GameOver::getNextScene()
{
	return nextScene;
}



void GameOver::initShaders()
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
