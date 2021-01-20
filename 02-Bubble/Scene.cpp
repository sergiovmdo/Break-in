#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define CHRONO_TIME 50000

float xLeft = 144.0;
float xRight = 270.0f;
float yTop = .0f;
float yBottom = 2000.0f;

std::vector<glm::vec2> coinBlocksCoords3 = { glm::ivec2(57 , 18) , glm::ivec2(88 , 18) , glm::ivec2(119 , 18) , glm::ivec2(150 , 18) , glm::ivec2(181 , 18) ,
											glm::ivec2(212 , 18) , glm::ivec2(243 , 18) , glm::ivec2(274 , 18) , glm::ivec2(305 , 18) ,glm::ivec2(336 , 18) };
std::vector<glm::vec2> bagBlocksCoords3 = { glm::ivec2(57 , 121), glm::ivec2(57 , 168) , glm::ivec2(88 , 168) , glm::ivec2(118 , 168) ,
											glm::ivec2(274 , 168) , glm::ivec2(305 , 168) , glm::ivec2(336 , 168) , glm::ivec2(336 , 121) };

Scene::Scene()
{
	map = NULL;
	player = NULL;
	ball = NULL;
	bricksMap = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	if (bricksMap != NULL)
		delete bricksMap;
}

void Scene::createIndestructibleBlocks(int rows, int columns, float x, float y, bool leftToRight, bool bothSides)
{
	pair<glm::ivec2, glm::ivec2> p;
	p.first = glm::ivec2(x, y);
	p.second = glm::ivec2(rows * 16, columns * 16);
	ball->addColisionAreas(glm::ivec2(x, y), glm::ivec2(rows * 16, columns * 16));
	player->indestructibleAreas.push_back(glm::vec4(x, y, x + 16 * columns, y + 16* rows));
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; ++j) {
			Block *block = new Block();

			switch (sceneNumber)
			{
			case 1:
				block->initIndestructibleBlocks(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				break;
			case 2:
				block->initIndestructibleBlocks2(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				break;
			case 3:
				block->initIndestructibleBlocks3(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				break;
			default:
				break;
			}

			block->setPosition(glm::ivec2(x + 16 * i, y + 16 * j));
			indestructibleBlocks.push_back(block);
			if ((leftToRight || bothSides) && i == columns - 1 && j == rows - 1)
				block->isCorner = true;
			if ((!leftToRight || bothSides) && i == 0 && j == 0)
				block->isCorner = true;

		}
	}
}

void Scene::activateGodMode()
{
	if (!godMode) {
		godMode = true;
		ball->velocity.x = 0;
	}
	else {
		godMode = false;
	}
}

void Scene::hookTheBall()
{
	ball->hooked = false;
	//player->size.y -= ball->size.y;
}

void Scene::checkIfButtonsAreFlying()
{
}

void Scene::initSceneLevel1()
{
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	loadBricks("levels/bricks01.txt");
	xLeft = 140.0;
	xRight = 260.0f;

	Audio::instance().stopAll();
	Audio::instance().play(LEVEL_1, true);
}

void Scene::initSceneLevel2()
{
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	loadBricks("levels/bricks02.txt");
	xLeft = 108.0;
	xRight = 292.0f;

	Audio::instance().stopAll();
	Audio::instance().play(LEVEL_2, true);
}

void Scene::initSceneLevel3()
{
	map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	loadBricks("levels/bricks03.txt");
	/*xLeft = 76.0;
	xRight = 324.0f;*/
	xLeft = 108.0;
	xRight = 292.0f;
	Audio::instance().stopAll();
	Audio::instance().play(LEVEL_3, true);
}



void Scene::init()
{
	initShaders();
	blocks = vector<Block*>();
	switch (sceneNumber) {
	case 1:
		initSceneLevel1();
		break;
	case 2:
		initSceneLevel2();
		break;
	case 3:
		initSceneLevel3();
		break;
	}

	switch (sceneNumber) {
	case 1:
		initTopGateBlocks();
		break;
	case 2:
		initTopGateBlocks2();
		break;
	case 3:
		initTopGateBlocks3();
		break;
	}

	switch (sceneNumber) {
	case 1:
		initKeyBlock();
		break;
	case 2:
		initKeyBlock2();
		break;
	case 3:
		initKeyBlock3();
		break;
	}

	initBagBlocks();
	initCoinBlocks();
	initAlarmBlocks();
	initLifes();

	//Init player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(190, 1010));
	player->setTileMap(map);

	//Init Putin
	putin = new Putin();
	putin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	putin->setPosition(glm::vec2(0, 0));
	putin->setTileMap(map);
	putin->player = player;

	//Init ball
	ball = new Ball();
	ball->setPositionPlayer(player->posPlayer);
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	initIndestrictibleBlocks();

	ball->setPosition(glm::vec2(player->posPlayer.x + 16, player->posPlayer.y - 32));
	ball->setTileMap(map);

	player->ball = ball;
	player->indestructibleBlocks = indestructibleBlocks;

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
	currentTime = 0.0f;

	points = 0;
	money = 0;

	if (!alarmText.init("fonts/DS-DIGII.ttf"))
		cout << "Could not load font!!!" << endl;

	if (!scoreText.init("fonts/ArameMono-Stencil.ttf"))
		cout << "Could not load font!!!" << endl;

	alarmActivated = false;
	endGame = false;
	goToNextScene = false;
	guardsActivated = false;
	guardians.clear();

	setSceneToLevel1();
}


void Scene::initIndestrictibleBlocks()
{
	switch (sceneNumber) {
	case 1:
		ball->scene1 = true;
		ball->scene2 = false;
		ball->scene3 = false;
		createIndestructibleBlocks(2, 6, 16, 128, true, false);
		createIndestructibleBlocks(2, 6, 304, 128, false, false);
		break;
	case 2:
		ball->scene2 = true;
		ball->scene1 = false;
		ball->scene3 = false;
		//Pantalla 1
		createIndestructibleBlocks(1, 3, 16, 800, true, false);
		//createIndestructibleBlocks(1, 3, 16, 880, true, false);
		createIndestructibleBlocks(1, 3, 352, 800, false, false);
		//createIndestructibleBlocks(1, 3, 352, 880, false, false);

		createIndestructibleBlocks(1, 4, 101, 906, true, true);
		createIndestructibleBlocks(1, 4, 253, 906, false, true);

		//Pantalla 2
		createIndestructibleBlocks(8, 2, 115, 446, true, true);
		createIndestructibleBlocks(8, 2, 272, 446, false, true);
		createIndestructibleBlocks(1, 3, 185, 554, true, true);

		//Pantalla 3
		createIndestructibleBlocks(1, 4, 16, 96, true, false);
		createIndestructibleBlocks(1, 4, 336, 96, false, false);
		createIndestructibleBlocks(1, 4, 176, 142, true, true);
		createIndestructibleBlocks(1, 4, 336, 192, false, false);
		createIndestructibleBlocks(1, 4, 16, 192, true, false);

		break;
	case 3:
		ball->scene3 = true;
		ball->scene2 = false;
		ball->scene1 = false;
		//Pantalla 1
		createIndestructibleBlocks(1, 3, 184, 818, true, true);
		//Pantalla 3
		createIndestructibleBlocks(1, 8, 16, 80, true, false);
		createIndestructibleBlocks(1, 8, 272, 80, false, false);
		createIndestructibleBlocks(1, 8, 16, 192, true, false);
		createIndestructibleBlocks(1, 8, 272, 192, false, false);
		break;
	}
}

void Scene::scrollToNextScreen()
{

	if (canScrollToNextScreen()) {
		float lastDy = dy;
		currentScrollingPosition = lastDy;
		isScrolling = true;
		isScrollingToNextScreen = true;
		if (level1) {
			dy = 352.5;
			scrollingDestination = dy;
			yBottom = 658;
			level1 = false;
			ball->level1 = false;
			level2 = true;
			ball->level2 = true;
			player->setPosition(glm::vec2(190, 660));
			ball->posBall.y -= 100;

			for (int i = 0; i < lifes.size(); ++i) {
				if (!lifes[i]->isDestroyed)
					lifes[i]->setPosition(glm::ivec2(lifes[i]->pos.x, lifes[i]->pos.y - 352.5));
			}
		}
		else if (level2) {
			dy = 0.0;
			scrollingDestination = dy;
			yBottom = 300;
			level2 = false;
			ball->level2 = false;
			level3 = true;
			ball->level3 = true;
			player->setPosition(glm::vec2(190, 300));
			ball->posBall.y -= 100;

			for (int i = 0; i < lifes.size(); ++i) {
				if (!lifes[i]->isDestroyed)
					lifes[i]->setPosition(glm::ivec2(lifes[i]->pos.x, lifes[i]->pos.y - 352.5));
			}
		}
		/*int cont = 0;
		for (float i = lastDy + 1000; i >= dy; i--) {
			cont++;
			if (cont % 10 == 0)
				projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + i, i);
		}*/

	}
}

void Scene::scrollAnimToPreviousScreen()
{
	if ((currentScrollingPosition >= scrollingDestination)) {
		isScrolling = false;
		isScrollingToNextScreen = false;
		isScrollingToPreviousScreen = false;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
	}
	else {
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + currentScrollingPosition, currentScrollingPosition);
		currentScrollingPosition += 10;
	}
	
}

void Scene::scrollAnimToNextScreen()
{
	if ((currentScrollingPosition <= scrollingDestination)) {
		isScrolling = false;
		isScrollingToNextScreen = false;
		isScrollingToPreviousScreen = false;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
	}
	else {
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + currentScrollingPosition, currentScrollingPosition);
		currentScrollingPosition -= 10;
	}

}

void Scene::scrollToPreviousScreen()
{
	float lastDy = dy;
	if (canScrollToPreviousScreen()) {
		isScrollingToPreviousScreen = true;
		currentScrollingPosition = lastDy;
		isScrolling = true;
		
		if (level2) {
			dy = 705.0;
			scrollingDestination = dy;
			yBottom = 2000;
			level2 = false;
			ball->level2 = false;
			level1 = true;
			ball->level1 = true;
			player->setPosition(glm::vec2(190, 1010));
			ball->posBall.y += 100;

			for (int i = 0; i < lifes.size(); ++i) {
				if (!lifes[i]->isDestroyed)
					lifes[i]->setPosition(glm::ivec2(lifes[i]->pos.x, lifes[i]->pos.y + 352.5));
			}
		}
		else if (level3) {
			dy = 352.5;
			scrollingDestination = dy;
			yBottom = 658;
			level2 = true;
			ball->level2 = true;
			level3 = false;
			ball->level3 = false;
			player->setPosition(glm::vec2(190, 660));
			ball->posBall.y += 100;

			for (int i = 0; i < lifes.size(); ++i) {
				if (!lifes[i]->isDestroyed)
					lifes[i]->setPosition(glm::ivec2(lifes[i]->pos.x, lifes[i]->pos.y + 352.5));
			}
		}
	}
}



bool Scene::canScrollToPreviousScreen()
{
	if (ball->posBall.y >= yBottom) {
		if (sceneNumber == 3 && level2) {
			//El agujerillo isquierdo
			float xLeft2 = xLeft - 96;
			float xRight2 = xLeft - 64;
			if (ball->posBall.x >= xLeft2 && ball->posBall.x <= xRight2) {
				return true;
			}
			//El agujerillo derexo
			float xRight3 = xRight + 96;
			float xLeft3 = xRight + 64;

			if (ball->posBall.x >= xLeft3 && ball->posBall.x <= xRight3) {
				return true;
			}

			if (ball->posBall.x >= xRight2 + 32 && ball->posBall.x <= xLeft3 - 32)
				return true;
		}
		else if (ball->posBall.x >= xLeft && ball->posBall.x <= xRight)
			return true;
	}
	return false;
}

bool Scene::canScrollToNextScreen()
{
	if ((level1 && keyLevel1->isDestroyed) || (level2 && keyLevel2->isDestroyed)) {
		if (ball->posBall.y <= dy + 15) {
			if (ball->posBall.x >= 144.0 && ball->posBall.x <= 288.0)
				return true;
		}
	}
	return false;
}

void Scene::initBagBlocks()
{
	bagBlocks.clear();
	switch (sceneNumber) {
	case 1:
		for (int i = 0; i < bagBlocksCoords1.size(); ++i) {
			Block *block = new Block();
			block->initBagBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(bagBlocksCoords1[i]);
			bagBlocks.push_back(block);
		}
		break;
	case 2:
		for (int i = 0; i < bagBlocksCoords2.size(); ++i) {
			Block *block = new Block();
			block->initBagBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(bagBlocksCoords2[i]);
			bagBlocks.push_back(block);
		}
		break;
	case 3:
		for (int i = 0; i < bagBlocksCoords3.size(); ++i) {
			Block *block = new Block();
			block->initDiamondBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(bagBlocksCoords3[i]);
			bagBlocks.push_back(block);
		}
		break;
	}

}

void Scene::initCoinBlocks()
{
	coinBlocks.clear();
	switch (sceneNumber) {
	case 1:
		for (int i = 0; i < coinBlocksCoords1.size(); ++i) {
			Block *block = new Block();
			block->initCoinBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(coinBlocksCoords1[i]);
			coinBlocks.push_back(block);
		}
		break;
	case 2:
		for (int i = 0; i < coinBlocksCoords2.size(); ++i) {
			Block *block = new Block();
			block->initCoinBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(coinBlocksCoords2[i]);
			coinBlocks.push_back(block);
		}
		break;
	case 3:
		for (int i = 0; i < coinBlocksCoords3.size(); ++i) {
			Block *block = new Block();
			block->initCoinBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			block->setPosition(coinBlocksCoords3[i]);
			coinBlocks.push_back(block);
		}
		break;
	}
}

void Scene::initAlarmBlocks()
{
	switch (sceneNumber) {
	case 1:
		for (int i = 0; i < alarmBlocksCoords1.size(); ++i) {
			Block *block = new Block();
			if (i == 0) {
				block->initAlarmBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				alarmBlock = block;
			}
			else {
				block->initGuardActivationBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				guardBlock = block;
			}

			block->setPosition(alarmBlocksCoords1[i]);

		}
		break;
	case 2:
		for (int i = 0; i < alarmBlocksCoords2.size(); ++i) {
			Block *block = new Block();
			if (i == 0) {
				block->initAlarmBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				alarmBlock = block;
			}
			else {
				block->initGuardActivationBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				guardBlock = block;
			}

			block->setPosition(alarmBlocksCoords2[i]);

		}
		break;
	case 3:
		for (int i = 0; i < alarmBlocksCoords3.size(); ++i) {
			Block *block = new Block();
			if (i == 0) {
				block->initAlarmBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				alarmBlock = block;
			}
			else {
				block->initGuardActivationBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				guardBlock = block;
			}

			block->setPosition(alarmBlocksCoords3[i]);

		}
		break;
	}
}

void Scene::setSceneToLevel1()
{
	dy = 705.0;
	yBottom = 2000;
	level2 = false;
	level3 = false;
	ball->level2 = false;
	ball->level3 = false;
	level1 = true;
	ball->level1 = true;
	player->setPosition(glm::vec2(190, 1010));
	ball->setPosition(glm::vec2(ball->posBall.x, 950.0));

	for (int i = 0; i < lifes.size(); ++i) {
		if (!lifes[i]->isDestroyed)
			lifes[i]->setPosition(glm::ivec2(lifesCoords[i].x, lifesCoords[i].y));
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
}

void Scene::setSceneToLevel2()
{
	dy = 352.5;
	yBottom = 658;
	level1 = false;
	level3 = false;
	ball->level1 = false;
	ball->level3 = false;
	level2 = true;
	ball->level2 = true;
	player->setPosition(glm::vec2(190, 660));
	ball->setPosition(glm::vec2(ball->posBall.x, 640.0));

	for (int i = 0; i < lifes.size(); ++i) {
		if (!lifes[i]->isDestroyed)
			lifes[i]->setPosition(glm::ivec2(lifesCoords[i].x, lifesCoords[i].y - 352.5));
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
}

void Scene::setSceneToLevel3()
{
	dy = 0.0;
	yBottom = 300;
	level2 = false;
	level1 = false;
	ball->level2 = false;
	ball->level1 = false;
	level3 = true;
	ball->level3 = true;
	player->setPosition(glm::vec2(190, 305));
	ball->setPosition(glm::vec2(ball->posBall.x, 290.0));

	for (int i = 0; i < lifes.size(); ++i) {
		if (!lifes[i]->isDestroyed)
			lifes[i]->setPosition(glm::ivec2(lifesCoords[i].x, lifesCoords[i].y - (352.5 * 2)));
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) + dx, float(SCREEN_HEIGHT - 1) + dy, dy);
}


bool Scene::update(int deltaTime)
{
	if (!isScrolling) {

		currentTime += deltaTime;
		player->update(deltaTime);

		if (!ball->hooked && !ball->freezed) {
			deleteBlocksAtBallCollisions();
			detectPlayerBallCollisions();
			ball->update(deltaTime);
		}
		else if (!ball->freezed) {
			player->hookTheBall(ball);
			ball->updateSprite();
		}
		for (int i = 0; i < blocks.size(); i++) {
			blocks[i]->update(deltaTime);
		}

		keyLevel1->update(deltaTime);
		keyLevel2->update(deltaTime);

		for (int i = 0; i < lifes.size(); i++) {
			lifes[i]->update(deltaTime);
		}

		for (int i = 0; i < bagBlocks.size(); i++) {
			bagBlocks[i]->update(deltaTime);
		}

		for (int i = 0; i < coinBlocks.size(); i++) {
			coinBlocks[i]->update(deltaTime);
		}

		for (int i = 0; i < guardians.size(); i++) {
			guardians[i]->update(deltaTime);
		}

		if (guardsActivated) {
			for (int i = 0; i < indestructibleBlocks.size(); i++) {
				for (int j = 0; j < guardians.size(); ++j) {
					if (indestructibleBlocks[i]->checkColissionWithGuard(guardians[j]->pos, guardians[j]->size) && !indestructibleBlocks[i]->isDestroyed) {
						indestructibleBlocks[i]->isDestroyed = true;
						Audio::instance().play(ROCK, false);
					}
				}
			}

			for (int j = 0; j < guardians.size(); ++j) {
				if (alarmBlock->checkColissionWithGuard(guardians[j]->pos, guardians[j]->size)) {
					alarmBlock->isDestroyed = true;
				}
				if (guardBlock->checkColissionWithGuard(guardians[j]->pos, guardians[j]->size)) {
					guardBlock->isDestroyed = true;
				}
			}
		}

		scrollToNextScreen();
		scrollToPreviousScreen();

		if (putin->enabled) {
			putin->update(deltaTime);
		}
		if (!ball->hooked && !godMode)
			looseALife();

		if (player->deadAnimEnded) {
			setSceneToLevel1();
			player->deadAnimEnded = false;
			ball->freezed = false;
			ball->hooked = true;
			//player->size.y += ball->size.y;
		}

		if (putinAnim != NULL) {
			putinAnim->update(deltaTime);
		}

		if (putinAnim != NULL && putinAnim->putinAnimFinished) {
			putin->isAppearing = true;
			putin->sceneNumber = sceneNumber;
			putin->setPosition(glm::vec2(350, 270));
			if (sceneNumber == 3)
				putin->setPosition(glm::vec2(350, 950));
			putinAnim->putinAnimFinished = false;
			putin->enabled = true;
		}
		if (guardsActivated) {
			activateGuardians();
		}

		goToNextScene = false;
		if (level3 && levelFinished())
			goToNextScene = true;
	}
	else {
		if (isScrollingToNextScreen) {
			scrollAnimToNextScreen();
		}
		else {
			scrollAnimToPreviousScreen();
		}
	}
	return false;

}


void Scene::deleteBlocksAtBallCollisions() {
	if (!ball->hooked) {
		for (int i = 0; i < blocks.size(); i++) {
			glm::vec2 difference = blocks[i]->checkColission(ball);
			if (blocks[i]->checkColission2(ball) && !blocks[i]->isDestroyed && !blocks[i]->touched) {
				points += 100;
				bounceTheBallAtColidingWithBlocks(difference, blocks[i]);
				Audio::instance().play(ROCK, false);
				blocks[i]->deleteBlock();
				break;
			}
		}

		for (int i = 0; i < bagBlocks.size(); i++) {
			glm::vec2 difference = bagBlocks[i]->checkColission(ball);
			if (bagBlocks[i]->checkColission2(ball) && !bagBlocks[i]->isDestroyed && !bagBlocks[i]->touched) {
				if (sceneNumber == 3) {
					Audio::instance().play(DIAMOND, false);
					money += 500;
				}
				else {
					money += 200;
					Audio::instance().play(BAG, false);
				}
				bounceTheBallAtColidingWithBlocks(difference, bagBlocks[i]);
				bagBlocks[i]->deleteBlock();
				break;
			}
		}

		for (int i = 0; i < coinBlocks.size(); i++) {
			glm::vec2 difference = coinBlocks[i]->checkColission(ball);
			if (coinBlocks[i]->checkColission2(ball) && !coinBlocks[i]->isDestroyed && !coinBlocks[i]->touched) {
				Audio::instance().play(COIN, false);
				money += 100;
				bounceTheBallAtColidingWithBlocks(difference, coinBlocks[i]);
				coinBlocks[i]->deleteBlock();
				break;
			}
		}

		for (int i = 0; i < indestructibleBlocks.size(); i++) {
			glm::vec2 difference = indestructibleBlocks[i]->checkColission(ball);
			if (!indestructibleBlocks[i]->isDestroyed && indestructibleBlocks[i]->checkColission2(ball)) {
				Audio::instance().play(CHOQUE, false);
				bounceTheBallAtColidingWithBlocks(difference, indestructibleBlocks[i]);
				break;
			}
		}

		glm::vec2 difference = keyLevel1->checkColission(ball);
		if (keyLevel1->checkColission2(ball) && !keyLevel1->isDestroyed && !keyLevel1->touched) {
			Audio::instance().play(KEY, false);
			openTheGate();
		}
		if (keyLevel2->checkColission2(ball) && !keyLevel2->isDestroyed && !keyLevel2->touched) {
			Audio::instance().play(KEY, false);
			openTheGate();
		}

		if (alarmBlock->checkColission2(ball) && !alarmBlock->isDestroyed) {
			bounceTheBallAtColidingWithBlocks(glm::ivec2(0, 0), alarmBlock);
			risePutin();
			alarmBlock->activateButton();
		}

		if (guardBlock->checkColission2(ball) && !guardBlock->isDestroyed) {
			bounceTheBallAtColidingWithBlocks(glm::ivec2(0, 0), guardBlock);
			if (!guardsActivated) {
				time = currentTime;
				guardsActivated = true;
				activateAlarm();
			}
			activateGuardians();

			guardBlock->activateButton();
		}
	}
}



void Scene::detectPlayerBallCollisions()
{
	glm::vec2 difference = player->checkColission(ball);
	if (/*glm::length(difference) < ball->radius && keyLevel1->getReboundDirection(difference) == NORTHEAST*/ player->checkBlockColissionWithBall(ball) && ball->posBall.y <= player->posPlayer.y && ball->velocity.y > 0) {
		Audio::instance().play(PUNYETAZO_PLACA_METAL, false);
		bounceTheBallAtColidingWithPaddle(difference);
	}
}

void Scene::bounceTheBallAtColidingWithPaddle(glm::vec2 difference) {
	/*ball->velocity.y = -ball->velocity.y;
	float deepness = ball->radius - std::abs(difference.y);

	ball->posBall.y -= deepness;*/

	if (!godMode) {

		if (ball->velocity.x < 0)
			ball->velocity.x = -1.0;
		else
			ball->velocity.x = 1.0;

		float paddleCenter = player->posPlayer.x + (player->getSizePlayer().x / 2.0);
		float xball = ball->posBall.x;

		ball->velocity.y = -ball->velocity.y;
		if (paddleCenter == xball + ball->size.x / 2) {
			ball->velocity.x = 0;
			return;
		}
		//La bola va a chocar con la mitad derecha de la pala.
		if (paddleCenter < xball + ball->size.x / 2) {
			//Si la bola viene de la izquierda ya tiene la x positiva, no nos interesa cambiarla.
			if (ball->velocity.x < 0) {
				ball->velocity.x = -ball->velocity.x;
			}
			//En ambos casos queremos aplicarle una variación de la velocidad dependiendo del angulo de impacto
			float percentage = (xball - paddleCenter) / (player->getSizePlayer().x / 2.0);;
			ball->velocity.x += percentage * 2.0;
		}
		//La bola va a chocar con la mitad izquierda de la pala.
		else {
			//Si la bola viene de la derecha ya tiene la x negativa
			if (ball->velocity.x > 0) {
				ball->velocity.x = -ball->velocity.x;
			}

			float percentage = (paddleCenter - xball) / (player->getSizePlayer().x / 2.0);
			ball->velocity.x -= percentage * 1.5;
		}
	}
	else {
		ball->velocity.y = -ball->velocity.y;
	}
}

void Scene::bounceTheBallAtColidingWithBlocks(glm::vec2 difference, Block* block) {
	Direction d = block->getReboundDirection2(ball);
	float deepness;
	if (d == NORTHWEST || d == NORTHEAST || d == SOUTHWEST || d == SOUTHEAST) {

		if ((d == NORTHWEST || d == NORTHEAST)) {
			if (ball->velocity.y > 0)
				ball->velocity.y = -ball->velocity.y;

			if (block->isCorner)
				return;
		}
		else if (ball->velocity.y < 0) {
			ball->velocity.y = -ball->velocity.y;

			if (block->isCorner)
				return;
		}

		if (block->pos.x > ball->posBall.x) {
			if (ball->velocity.x > 0)
				ball->velocity.x = -ball->velocity.x;
			return;
		}
		else if (block->pos.x + block->size.x <= (ball->posBall.x + 5) && ball->velocity.x < 0) {
			ball->velocity.x = -ball->velocity.x;
			return;
		}

	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	//bricksMap->render();

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->render();
	}
	for (int i = 0; i < gateBlocksLevel1.size(); i++) {
		gateBlocksLevel1[i]->render();
	}
	for (int i = 0; i < gateBlocksLevel2.size(); i++) {
		gateBlocksLevel2[i]->render();
	}
	for (int i = 0; i < bagBlocks.size(); i++) {
		bagBlocks[i]->render();
	}
	for (int i = 0; i < coinBlocks.size(); i++) {
		coinBlocks[i]->render();
	}
	for (int i = 0; i < alarmBlocks.size(); i++) {
		alarmBlocks[i]->render();
	}
	for (int i = 0; i < lifes.size(); i++) {
		lifes[i]->render();
	}
	for (int i = 0; i < indestructibleBlocks.size(); i++) {
		indestructibleBlocks[i]->render();
	}

	for (int i = 0; i < guardians.size(); i++) {
		guardians[i]->render();
	}

	keyLevel1->render();
	keyLevel2->render();

	alarmBlock->render();
	guardBlock->render();

	player->render();
	putin->render();
	ball->render();
	alarmRender();

	if (putinAnim != NULL) {
		putinAnim->render();
	}

	scoreText.render("Points: " + std::to_string(points), glm::vec2(475, 100), 20, glm::vec4(1, 1, 1, 1));
	scoreText.render("Money: " + std::to_string(money), glm::vec2(475, 150), 20, glm::vec4(1, 1, 1, 1));
	string lvl = level1 ? "1" : level2 ? "2" : "3";
	scoreText.render("Level: " + std::to_string(sceneNumber) + ", section: " + lvl, glm::vec2(475, SCREEN_HEIGHT - 50), 10, glm::vec4(1, 1, 1, 1));

}

bool Scene::loadBricks(const string &levelFile)
{
	ifstream reader;
	reader.open(levelFile.c_str());
	string line;

	std::string delimiter = ";";
	while (getline(reader, line)) {
		size_t pos = 0;
		std::string localPosition;

		while ((pos = line.find(delimiter)) != std::string::npos) {
			localPosition = line.substr(0, pos);
			extractPositionFromString(localPosition);

			std::cout << localPosition << std::endl;
			if (pos + delimiter.length() < line.length())
				line.erase(0, pos + delimiter.length());
			else
				break;
		}

	}
	return false;
}

void Scene::extractPositionFromString(string s) {
	std::string delimiter = ",";
	string x = s.substr(0, s.find(','));
	s.erase(0, s.find(delimiter) + delimiter.length());
	string y = s.substr(0, s.find(','));
	s.erase(0, s.find(delimiter) + delimiter.length());
	string brickType = s.substr(0, s.find(','));
	int ix = stoi(x);
	int iy = stoi(y);
	int ibrickType = stoi(brickType);

	this->loadBrick(glm::ivec2(ix, iy), ibrickType);

}

bool Scene::levelFinished()
{
	//Esto se puede mejorar, pero de momento como no sabemos si tocaremos numero de bolsas/monedas se queda
	int cont = 0;
	for (int i = 0; i < bagBlocks.size(); i++) {
		if (bagBlocks[i]->isDestroyed)
			cont++;
	}
	for (int i = 0; i < coinBlocks.size(); i++) {
		if (coinBlocks[i]->isDestroyed)
			cont++;
	}

	return cont == (coinBlocks.size() + bagBlocks.size());
}

int Scene::paintBlocksAlgorithm()
{
	return 0;
}

void Scene::looseALife()
{
	if (ball->looseLife() || putin->checkColission()) {
		bool destroy = false;
		for (int i = lifes.size() - 1; i >= 0 && !destroy; --i) {
			if (!lifes[i]->isDestroyed) {
				lifes[i]->deleteBlock();
				destroy = true;
			}
		}
		if (!destroy)
			endGame = true;
		else {
			Audio::instance().play(LOOSE_LIFE, false);
			player->deadAnimation();
			ball->freezed = true;
			ball->hooked = true;
			//player->size.y += ball->size.y;
		}
	}


}

void Scene::loadBrick(glm::ivec2 pos, int brickNumber) {
	Block *block = new Block();
	if (brickNumber == 3) {
		block->initMultiHitBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	else if (brickNumber == 7) {
		block->initDoubleHitBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	else {
		block->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setBlockNumber(brickNumber);
	}
	block->setPosition(pos);

	blocks.push_back(block);

}

void Scene::initTopGateBlocks()
{
	int init = 144;
	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 704.0));
		gateBlocksLevel1.push_back(block);
	}

	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 352));
		gateBlocksLevel2.push_back(block);
	}

}

void Scene::initTopGateBlocks2()
{
	int init = 144;
	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks2(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 704.0));
		gateBlocksLevel1.push_back(block);
	}

	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks2(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 352));
		gateBlocksLevel2.push_back(block);
	}
}

void Scene::initTopGateBlocks3()
{
	int init = 144;
	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks3(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 704.0));
		gateBlocksLevel1.push_back(block);
	}

	for (int i = 0; i < 8; i++)
	{
		Block *block = new Block();
		block->initGateBlocks3(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		block->setPosition(glm::ivec2(init + 16 * i, 352));
		gateBlocksLevel2.push_back(block);
	}
}

void Scene::initLifes()
{
	lifes.clear();
	for (int i = 0; i < lifesCoords.size(); ++i) {
		Block *block = new Block();
		block->initLifeBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

		block->setPosition(lifesCoords[i]);

		lifes.push_back(block);
	}
}

void Scene::initKeyBlock()
{
	Block *block = new Block();
	block->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block->setPosition(keyPositions1[0]);
	keyLevel1 = block;

	Block *block2 = new Block();
	block2->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block2->setPosition(keyPositions1[1]);
	keyLevel2 = block2;
}

void Scene::initKeyBlock2()
{
	Block *block = new Block();
	block->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block->setPosition(keyPositions2[0]);
	keyLevel1 = block;

	Block *block2 = new Block();
	block2->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block2->setPosition(keyPositions2[1]);
	keyLevel2 = block2;
}

void Scene::initKeyBlock3()
{
	Block *block = new Block();
	block->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block->setPosition(keyPositions3[0]);
	keyLevel1 = block;

	Block *block2 = new Block();
	block2->initKeyBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block2->setPosition(keyPositions3[1]);
	keyLevel2 = block2;
}

void Scene::openTheGate()
{
	if (level1) {
		keyLevel1->deleteBlock();
		for (int i = 0; i < gateBlocksLevel1.size(); i++) {
			gateBlocksLevel1[i]->deleteBlock();
		}
	}
	if (level2) {
		keyLevel2->deleteBlock();
		for (int i = 0; i < gateBlocksLevel2.size(); i++) {
			gateBlocksLevel2[i]->deleteBlock();
		}
	}

}

void Scene::risePutin()
{
	if (!putin->risen) {
		putin->risen = true;
		putinAnim = new Block();
		Audio::instance().play(EXPLOSION, false);
		putinAnim->initPutinAnimBlock(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		putinAnim->setPosition(glm::vec2(350, 270));
		if (sceneNumber == 3)
			putinAnim->setPosition(glm::vec2(350, 950));
	}
}

void Scene::activateGuardians()
{
	if (currentTime - time > 5000) {
		Guard* guard = new Guard();
		guard->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		guard->setTileMap(map);
		guard->player = player;

		bool found = false;
		do {
			int v1 = rand() % bagBlocks.size();
			if (v1 < 0) {
				continue;
			}
			Block* objective = bagBlocks[v1];
			if (!objective->isObjective && !objective->isDestroyed) {
				objective->isObjective = true;
				guard->objective = objective;
				found = true;
			}
			if (!found) {
				bool allSelected = true;
				for (int i = 0; i < bagBlocks.size(); i++) {
					if (!bagBlocks[i]->isObjective && !objective->isDestroyed) {
						allSelected = false;
						objective->isObjective = true;
						guard->objective = objective;
						found = true;
					}
				}
				if (allSelected)
					break;
			}
		} while (!found);

		if (found) {
			guard->removeBag();

			guardians.push_back(guard);
		}
		else
			guard->activated = false;
		time = currentTime;
	}

}

void Scene::initShaders()
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

void Scene::activateAlarm()
{
	if (!alarmActivated)
	{
		alarmActivated = true;
		Audio::instance().play(ALARM, true);
		alarmTimeStart = currentTime + CHRONO_TIME;
	}
}
void Scene::alarmUpdate()
{
	if (alarmActivated)
	{
		if (currentTime - alarmLastDeletedMoney >= timeBetweenDeletions)
		{
			if (!coinBlocks.empty() && !bagBlocks.empty())
			{
				if (rand() % 2 == 1)
				{
					coinBlocks.erase(coinBlocks.begin() + (rand() % coinBlocks.size()));
				}
				else
				{
					bagBlocks.erase(bagBlocks.begin() + (rand() % bagBlocks.size()));
				}
				alarmLastDeletedMoney = currentTime;
			}
			else if (!coinBlocks.empty())
			{
				coinBlocks.erase(coinBlocks.begin() + (rand() % coinBlocks.size()));
				alarmLastDeletedMoney = currentTime;
			}
			else if (!bagBlocks.empty())
			{
				bagBlocks.erase(bagBlocks.begin() + (rand() % bagBlocks.size()));
				alarmLastDeletedMoney = currentTime;
			}
		}
	}
}

void Scene::alarmRender()
{
	if (alarmActivated)
	{
		float timeLeft = alarmTimeStart - currentTime;
		if (timeLeft >= 0)
		{
			int minutes = timeLeft / 60000;
			timeLeft -= minutes * 60000;
			int seconds = timeLeft / 1000;
			timeLeft -= seconds * 1000;
			int mili = timeLeft;
			string timeLeftString = std::to_string(minutes) + ":" + std::to_string(seconds) + ":" + std::to_string(mili);
			alarmText.render(timeLeftString, glm::vec2(475, 250), 40, glm::vec4(1, 0, 0, 1));
		}
		else
		{
			alarmActivated = false;
			guardsActivated = false;
			for (auto& coinBlock : coinBlocks)
			{
				if (!coinBlock->isDestroyed)
				{
					endGame = true;
					return;
				}
			}

			for (auto& bagBlock : bagBlocks)
			{
				if (!bagBlock->isDestroyed)
				{
					endGame = true;
					return;
				}
			}

			goToNextScene = true;
		}
	}
}

int Scene::getNextScene()
{
	if (goToNextScene)
	{
		goToNextScene = false;
		switch (sceneNumber)
		{
		case GAME_SCENE_1:
			return LEVEL_PASSED_SCENE_1;
		case GAME_SCENE_2:
			return LEVEL_PASSED_SCENE_2;
		case GAME_SCENE_3:
			return LEVEL_PASSED_SCENE_3;
		}
	}

	if (endGame)
	{
		endGame = false;
		switch (sceneNumber)
		{
		case GAME_SCENE_1:
			return GAME_OVER_SCENE_1;
		case GAME_SCENE_2:
			return GAME_OVER_SCENE_2;
		case GAME_SCENE_3:
			return GAME_OVER_SCENE_3;
		}
	}

	return sceneNumber;
}
