#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"
#include "Putin.h"
#include "Text.h"
#include "Guard.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();
	Block* putinAnim;
	Block* portal;
	int sceneNumber;
	int money = 0;
	int points = 0;
	bool nextLevel = false;
	bool level1 = true;
	bool level2 = false;
	bool level3 = false;
	float dx = 0.0;
	float dy = 705.0;
	bool guardsActivated = false;
	float time;



	bool isScrolling = false;
	bool isScrollingToNextScreen = false;
	bool isScrollingToPreviousScreen = false;
	float currentScrollingPosition;
	float scrollingDestination;

	void scrollAnimToNextScreen();
	void scrollAnimToPreviousScreen();

	Ball *ball;
	Player *player;
	Putin *putin;
	bool godMode = false;
	void init();
	bool update(int deltaTime);
	void render();
	bool loadBricks(const string &levelFile);
	void deleteBlocksAtBallCollisions();
	void detectPlayerBallCollisions();
	void bounceTheBallAtColidingWithPaddle(glm::vec2 difference);
	void bounceTheBallAtColidingWithBlocks(glm::vec2 difference, Block* block);
	void extractPositionFromString(string s);

	bool levelFinished();

	void setSceneToLevel1();
	void setSceneToLevel2();
	void setSceneToLevel3();

	int paintBlocksAlgorithm();

	void looseALife();
	void activateGodMode();
	void hookTheBall();

	void checkIfButtonsAreFlying();

	int getNextScene();

private:
	void initShaders();

private:
	TileMap *map;
	TileMap *bricksMap;
	int levelNum = 0;
	std::vector<glm::ivec2> keyPositions1 = {glm::ivec2(116,800), glm::ivec2(79,482) };
	std::vector<glm::ivec2> keyPositions2 = { glm::ivec2(20,820), glm::ivec2(145,520) };
	std::vector<glm::ivec2> keyPositions3 = { glm::ivec2(176,836), glm::ivec2(30,418) };
	std::vector<Block*> blocks;
	std::vector<Block*> indestructibleBlocks;
	std::vector<Guard*> guardians;
	
	std::vector<Block*> lifes;
	std::vector<glm::vec2> lifesCoords = { glm::ivec2(430.0 , 720.0) , glm::ivec2(463.0 , 720.0) , glm::ivec2(496.0 , 720.0) , glm::ivec2(529.0 , 720.0) , glm::ivec2(562.0 , 720.0)};
	
	std::vector<Block*> bagBlocks;
	std::vector<glm::vec2> bagBlocksCoords1 = { glm::ivec2(45.0 , 70.0), glm::ivec2(45.0 , 97.0), glm::ivec2(70.0 , 70.0) , glm::ivec2(70.0 , 97.0) , glm::ivec2(276, 70) , glm::ivec2(301, 70) , glm::ivec2(276 , 97) , glm::ivec2(301, 97) , glm::ivec2(45, 23) , glm::ivec2(70, 23) , glm::ivec2(95, 23) , glm::ivec2(120, 23) };
	std::vector<glm::vec2> bagBlocksCoords2 = { glm::ivec2(58.0 , 71.0), glm::ivec2(17.0 , 71.0), glm::ivec2(335.0 , 71.0), glm::ivec2(377.0 , 71.0) ,
													glm::ivec2(58.0 , 167), glm::ivec2(17.0 , 167), glm::ivec2(335.0 , 167), glm::ivec2(377.0 , 167) };

	std::vector<glm::vec2> coinBlocksCoords3 = { glm::ivec2(57 , 18) , glm::ivec2(88 , 18) , glm::ivec2(119 , 18) , glm::ivec2(150 , 18) , glm::ivec2(181 , 18) ,
		glm::ivec2(212 , 18) , glm::ivec2(243 , 18) , glm::ivec2(274 , 18) , glm::ivec2(305 , 18) ,glm::ivec2(336 , 18) };

	std::vector<glm::vec2> bagBlocksCoords3 = { glm::ivec2(57 , 121), glm::ivec2(57 , 168) , glm::ivec2(88 , 168) , glm::ivec2(118 , 168) ,
		glm::ivec2(274 , 168) , glm::ivec2(305 , 168) , glm::ivec2(336 , 168) , glm::ivec2(336 , 121) };
	
	std::vector<Block*> coinBlocks;
	std::vector<glm::vec2> coinBlocksCoords1 = { glm::ivec2(93.0 , 70.0), glm::ivec2(93.0 , 97.0), glm::ivec2(117.0 , 70.0) , glm::ivec2(117.0 , 97.0) , glm::ivec2(324.0 , 70) , glm::ivec2(349.0 , 70) , glm::ivec2(324.0 , 97.0) , glm::ivec2(349.0 , 97.0) , glm::ivec2(282, 23) , glm::ivec2(314, 23) , glm::ivec2(346, 23) };
	std::vector<glm::vec2> coinBlocksCoords2 = { glm::ivec2(179 , 192), glm::ivec2(214 , 192), glm::ivec2(178, 63) , glm::ivec2(214, 63) };
	
	
	std::vector<glm::vec2> alarmBlocksCoords1 = { glm::ivec2(42.0 ,160), glm::ivec2(345.0 , 160) };
	std::vector<glm::vec2> alarmBlocksCoords2 = { glm::ivec2(42.0 ,208), glm::ivec2(345.0 , 208) };
	std::vector<glm::vec2> alarmBlocksCoords3 = { glm::ivec2(208, 834), glm::ivec2(345.0 , 208) };
	
	std::vector<Block*> gateBlocksLevel1;
	std::vector<Block*> gateBlocksLevel2;
	std::vector<Block*> alarmBlocks;
	Block* alarmBlock;
	Block* guardBlock;

	bool endGame = false;
	
	Block* keyLevel1;
	Block* keyLevel2;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Text scoreText;

	
	void loadBrick(glm::ivec2 pos, int brickNumber);
	void initTopGateBlocks();
	void initTopGateBlocks2();
	void initTopGateBlocks3();
	void initLifes();
	void initKeyBlock();
	void initKeyBlock2();
	void initKeyBlock3();

	void activateGuardians();
	void openTheGate();

	void risePutin();
	
	void scrollToNextScreen();
	bool canScrollToNextScreen();

	void scrollToPreviousScreen();
	bool canScrollToPreviousScreen();

	void initBagBlocks();
	void initCoinBlocks();
	void initAlarmBlocks();

	void initSceneLevel1();
	void initSceneLevel2();
	void initSceneLevel3();

	void createIndestructibleBlocks(int rows, int columns, float x, float y, bool leftToRight, bool bothSides);

	void initIndestrictibleBlocks();

	void activateAlarm();
	void alarmUpdate();
	void alarmRender();
	bool alarmActivated;
	float alarmLastDeletedMoney;
	float alarmTimeStart;
	Text  alarmText;
	int timeBetweenDeletions = 2000;
	
	bool goToNextScene;
};


#endif // _SCENE_INCLUDE

