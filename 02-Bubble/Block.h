#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Ball.h"

enum Direction {
	NORTHWEST,
	NORTHEAST,
	SOUTHWEST,
	SOUTHEAST,
	WEST,
	EAST,
	NONE
};

enum DestroyState {
	SHAKE,
	FALL
};


class Block {
	public:
		int animCountDown = 10;
		int hits = 3;
		float time = .0;
		bool isMultiHit = false;
		bool isDoubleHit = false;
		bool touched = false;
		bool isGuardianBlock = false;
		bool isObjective = false;
		bool buttonPressed = false;
		bool putinAnimFinished = false;
		bool isPutinAnim = false;

		bool isCorner = false;

		glm::vec2 checkColission(Ball* ball);
		bool checkColission2(Ball* ball);
		bool checkColissionWithGuard(glm::ivec2 pos, glm::ivec2 size);
		bool checkTopColission(glm::ivec2 pos, glm::ivec2 size);
		bool checkBotColission(glm::ivec2 pos, glm::ivec2 size);
		bool checkRightColission(glm::ivec2 pos, glm::ivec2 size);
		bool checkLeftColission(glm::ivec2 pos, glm::ivec2 size);
	
		void setPosition(const glm::vec2 &pos);
		glm::ivec2 size = glm::ivec2(32.0, 16.0);
		void reset(glm::vec2 playerPosition);
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initGateBlocks(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initGateBlocks2(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initGateBlocks3(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initIndestructibleBlocks(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initIndestructibleBlocks2(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initIndestructibleBlocks3(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initKeyBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initBagBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initCoinBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initAlarmBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initLifeBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initDiamondBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initGuardActivationBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initPutinAnimBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initMultiHitBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void initDoubleHitBlock(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);

		void initPortalPart1(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);

		void update(int deltaTime);
		void render();

		void updatePutinAnim(int deltaTime);

		void activateButton();

		Direction getReboundDirection(glm::vec2 difference);
		Direction getReboundDirection2(Ball *ball);
		void setBlockNumber(int blockNumber);

		bool isGateBlock = false;
		bool isKey = false;
		bool isAlarm = false;
		bool isDestroyed = false;
		bool isIndestructible = false;
		bool isLife = false;

		void makeAnimation();

		void deleteBlock();
		glm::ivec2 pos;

	private:
		glm::ivec2 tileMapDispl;
		Texture spritesheet;
		Sprite *sprite;
		

};

#endif