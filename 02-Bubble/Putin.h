#ifndef _PUTIN_INCLUDE
#define _PUTIN_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Block.h"

enum Quarter {
	TOPRIGHT,
	TOPLEFT,
	BOTTOMRIGHT,
	BOTTOMLEFT,
};

class Putin
{

public:
	bool enabled = false;
	bool risen = false;
	float time = 0.0;
	int sceneNumber;
	bool isAppearing = false;
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 size = glm::ivec2(36.0, 8.0), pos, velocity = glm::ivec2(1, 1);
	Player* player;
	bool checkColission();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	Quarter getPlayerQuarter();
	Quarter getPutinQuarter();

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif