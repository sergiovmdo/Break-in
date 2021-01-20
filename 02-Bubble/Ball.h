#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Ball
{
	public:
		bool hooked = true;
		bool freezed = false;
		float radius = 12.0;
		glm::ivec2 size;
		glm::vec2 posBall;
		TileMap *bricksMap;

		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void update(int deltaTime);
		void render();

		void setTileMap(TileMap *tileMap);
		void setPosition(const glm::vec2 &pos);
		void setPositionPlayer(const glm::vec2 &pos);

		std::vector<pair<glm::ivec2, glm::ivec2>> colisionAreas;

		void reset(glm::vec2 playerPosition);
		glm::vec2 velocity;
		void deleteBrick();
		Sprite *sprite;
		void updateSprite();

		bool looseLife();

		void addColisionAreas(glm::ivec2 area, glm::ivec2 size);

		void detectCollisioningWithIndestructibleBlocks();

		bool level1 = true;
		bool level2;
		bool level3;


		bool scene1;
		bool scene2;
		bool scene3;

	private:
		glm::ivec2 tileMapDispl, posPlayer;
		
		Texture spritesheet;
		
		TileMap *map;


};

#endif