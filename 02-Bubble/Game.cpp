#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	initScenes();
}

void Game::initScenes()
{
	switch (currentScene)
	{
	default:
	case MENU_SCENE:
		menuScene.init();
		break;
	case GAME_SCENE_1:
		sceneLevel1.sceneNumber = GAME_SCENE_1;
		sceneLevel1.init();
		break;
	case GAME_SCENE_2:
		sceneLevel2.sceneNumber = GAME_SCENE_2;
		sceneLevel2.init();
		break;
	case GAME_SCENE_3:
		sceneLevel3.sceneNumber = GAME_SCENE_3;
		sceneLevel3.init();
		break;
	case CREDITS_SCENE:
		creditsScene.init();
		break;
	case INSTRUCTIONS_SCENE:
		instructionsScene.init();
		break;
	case LEVEL_PASSED_SCENE_1:
	case LEVEL_PASSED_SCENE_2:
	case LEVEL_PASSED_SCENE_3:
		levelPassedScene.init(currentScene);
		break;
	case GAME_OVER_SCENE_1:
	case GAME_OVER_SCENE_2:
	case GAME_OVER_SCENE_3:
		gameOverScene.init(currentScene);
		break;
		
	}
}

bool Game::update(int deltaTime)
{
	pollNextScene();

	switch (currentScene)
	{
	default:
	case MENU_SCENE:
		menuScene.update(deltaTime);
		break;
	case GAME_SCENE_1:
		sceneLevel1.update(deltaTime);
		break;
	case GAME_SCENE_2:
		sceneLevel2.update(deltaTime);
		break;
	case GAME_SCENE_3:
		sceneLevel3.update(deltaTime);
		break;
	case CREDITS_SCENE:
		creditsScene.update(deltaTime);
		break;
	case INSTRUCTIONS_SCENE:
		instructionsScene.update(deltaTime);
		break;
	case LEVEL_PASSED_SCENE_1:
	case LEVEL_PASSED_SCENE_2:
	case LEVEL_PASSED_SCENE_3:
		levelPassedScene.update(deltaTime);
		break;
	case GAME_OVER_SCENE_1:
	case GAME_OVER_SCENE_2:
	case GAME_OVER_SCENE_3:
		gameOverScene.update(deltaTime);
		break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (currentScene)
	{
	default:
	case MENU_SCENE:
		menuScene.render();
		break;
	case GAME_SCENE_1:
		sceneLevel1.render();
		break;
	case GAME_SCENE_2:
		sceneLevel2.render();
		break;
	case GAME_SCENE_3:
		sceneLevel3.render();
		break;
	case CREDITS_SCENE:
		creditsScene.render();
		break;
	case INSTRUCTIONS_SCENE:
		instructionsScene.render();
		break;
	case LEVEL_PASSED_SCENE_1:
	case LEVEL_PASSED_SCENE_2:
	case LEVEL_PASSED_SCENE_3:
		levelPassedScene.render();
		break;
	case GAME_OVER_SCENE_1:
	case GAME_OVER_SCENE_2:
	case GAME_OVER_SCENE_3:
		gameOverScene.render();
		break;
	case EXIT_SCENE:
		bPlay = false;
	}
}

void Game::keyPressed(int key)
{
	switch (currentScene)
	{
	case MENU_SCENE:
		if (key == 13) menuScene.enterPressed();
		break;
	case CREDITS_SCENE:
		if (key == 13) creditsScene.enterPressed();
		break;
	case INSTRUCTIONS_SCENE:
		if (key == 13) instructionsScene.enterPressed();
		break;
	case LEVEL_PASSED_SCENE_1:
	case LEVEL_PASSED_SCENE_2:
	case LEVEL_PASSED_SCENE_3:
		if (key == 13) levelPassedScene.enterPressed();
		break;
	case GAME_OVER_SCENE_1:
	case GAME_OVER_SCENE_2:
	case GAME_OVER_SCENE_3:
		if (key == 13) gameOverScene.enterPressed();
		break;
	default:
		if (key == 27) {// Escape code
			currentScene = MENU_SCENE;
			initScenes();
			return;
		}
		if (key == 32) //Spacebar code
			switch (currentScene)
			{
			case 1:
				sceneLevel1.hookTheBall();
				break;
			case 2:
				sceneLevel2.hookTheBall();
				break;
			case 3:
				sceneLevel3.hookTheBall();
				break;
			}
		if (key == 49) //Number 1
			switch (currentScene)
			{
			case 1:
				sceneLevel1.setSceneToLevel1();
				break;
			case 2:
				sceneLevel2.setSceneToLevel1();
				break;
			case 3:
				sceneLevel3.setSceneToLevel1();
				break;
			}

		if (key == 50) //Number 2
			switch (currentScene)
			{
			case 1:
				sceneLevel1.setSceneToLevel2();
				break;
			case 2:
				sceneLevel2.setSceneToLevel2();
				break;
			case 3:
				sceneLevel3.setSceneToLevel2();
				break;
			}
		if (key == 51) //Number 3
			switch (currentScene)
			{
			case 1:
				sceneLevel1.setSceneToLevel3();
				break;
			case 2:
				sceneLevel2.setSceneToLevel3();
				break;
			case 3:
				sceneLevel3.setSceneToLevel3();
				break;
			}
		if (key == 52) { //Scene 1 Number 4
			currentScene = GAME_SCENE_1;
			changeToNextLevel();
		}
		if (key == 53) {//Scene 2 Number 5
			currentScene = GAME_SCENE_2;
			changeToNextLevel();
		}
		if (key == 54) { //Scene 3 Number 6
			currentScene = GAME_SCENE_3;
			changeToNextLevel();
		}
		if (key == 'G' || key == 'g') {//G
			switch (currentScene)
			{
			case 1:
				sceneLevel1.activateGodMode();
				break;
			case 2:
				sceneLevel2.activateGodMode();
				break;
			case 3:
				sceneLevel3.activateGodMode();
			}
		}
		if (key == 'h' || key == 'H') {//H
			switch (currentScene)
			{
			case 1:
				sceneLevel1.ball->hooked = true;
				break;
			case 2:
				sceneLevel2.ball->hooked = true;
				break;
			case 3:
				sceneLevel3.ball->hooked = true;
				break;
			}
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (currentScene == MENU_SCENE)
	{
		if (key >= 'a' && key <= 'z')
		{
			menuScene.charPressed(key - ('a' - 'A'));
		}
		else if (key >= 'A' && key <= 'Z')
		{
			menuScene.charPressed(key);
		}
		else if (key == 8)
		{
			menuScene.removeChar();
		}
	}
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::changeToNextLevel()
{
	this->init();

}

void Game::pollNextScene()
{
	int oldScene = currentScene;
	switch (currentScene)
	{
	default:
	case MENU_SCENE:
		currentScene = menuScene.getNextScene();
		break;
	case GAME_SCENE_1:
		currentScene = sceneLevel1.getNextScene();
		break;
	case GAME_SCENE_2:
		currentScene = sceneLevel2.getNextScene();
		break;
	case GAME_SCENE_3:
		currentScene = sceneLevel3.getNextScene();
		break;
	case CREDITS_SCENE:
		currentScene = creditsScene.getNextScene();
		break;
	case INSTRUCTIONS_SCENE:
		currentScene = instructionsScene.getNextScene();
		break;
	case LEVEL_PASSED_SCENE_1:
	case LEVEL_PASSED_SCENE_2:
	case LEVEL_PASSED_SCENE_3:
		currentScene = levelPassedScene.getNextScene();
		break;
	case GAME_OVER_SCENE_1:
	case GAME_OVER_SCENE_2:
	case GAME_OVER_SCENE_3:
		currentScene = gameOverScene.getNextScene();
		break;
	}

	if (oldScene != currentScene)
		initScenes();
}





