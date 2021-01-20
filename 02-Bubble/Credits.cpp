#include "Credits.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Audio.h"


Credits::Credits()
{
	credits = NULL;
}


Credits::~Credits()
{
	if (credits != NULL)
	{
		delete credits;
	}
}

void Credits::init()
{
	initShaders();

	credits = TileMap::createTileMapMenu("levels/scenes/credits.txt", glm::vec2(0, 0), texProgram);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	nextScene = CREDITS_SCENE;
}

void Credits::render()
{

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	auto modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	credits->render();
}

void Credits::update(int deltaTime)
{
	currentTime += deltaTime;

	if (enterPress)
	{
		enterPress = false;
		Audio::instance().play(MENU_CLICK, false);
		nextScene = MENU_SCENE;
	}
}

void Credits::enterPressed()
{
	enterPress = true;
}

int Credits::getNextScene()
{
	return nextScene;
}


void Credits::initShaders()
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

