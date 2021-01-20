#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Credits.h"
#include "GameOver.h"
#include "Instructions.h"
#include "LevelPassed.h"
#include "Menu.h"
#include "Scene.h"
#include "Scenes.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360


// Game is a singleton (a class with a single instance) that represents our whole application

class Game
{

public:
	Game() {}
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	void pollNextScene();
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	void initScenes();
	void changeToNextLevel();
	
	bool bPlay;                       // Continue to play game?
	Scene sceneLevel1;                // Scene to render
	Scene sceneLevel2;
	Scene sceneLevel3;
	Menu menuScene;
	Credits creditsScene;
	Instructions instructionsScene;
	LevelPassed levelPassedScene;
	GameOver gameOverScene;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	

	int currentScene = MENU_SCENE;
};


#endif // _GAME_INCLUDE


