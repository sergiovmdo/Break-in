#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Ball.h"
#include "Block.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum Direction2 {
	UP,
	DOWN,
	RIGHTSIDE,
	LEFTSIDE,
	NONES
};

class Player
{

public:
	bool isDead = false;
	bool deadAnimEnded = false;
	float time = 0.0;
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 posPlayer;
	glm::ivec2 getSizePlayer();
	std::vector<Block*> indestructibleBlocks;
	Ball* ball;
	glm::vec2 checkColission(Ball* ball);
	bool checkBlockColission(Block* block);
	bool checkBlockColissionWithBall(Ball* block);
	Direction2 detectCollisioningWithIndestructibleBlocks();
	Direction getReboundDirection(glm::vec2 difference);
	void hookTheBall(Ball* ball);

	std::vector<glm::vec4> indestructibleAreas;

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void deadAnimation();
	glm::ivec2 size = glm::ivec2(32.0, 8.0);
private:
	
	bool bJumping;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


