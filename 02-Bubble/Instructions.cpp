#include "Instructions.h"


#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Audio.h"


Instructions::Instructions()
{
	credits = NULL;
}


Instructions::~Instructions()
{
	if (credits != NULL)
	{
		delete credits;
	}
}

void Instructions::init()
{
	initShaders();

	credits = TileMap::createTileMapMenu("levels/scenes/instructions.txt", glm::vec2(0, 0), texProgram);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	nextScene = INSTRUCTIONS_SCENE;
}

void Instructions::render()
{

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	auto modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	credits->render();
}

void Instructions::update(int deltaTime)
{
	currentTime += deltaTime;

	if (enterPress)
	{
		enterPress = false;
		Audio::instance().play(MENU_CLICK, false);
		nextScene = MENU_SCENE;
	}
}

void Instructions::enterPressed()
{
	enterPress = true;
}

int Instructions::getNextScene()
{
	return nextScene;
}


void Instructions::initShaders()
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


