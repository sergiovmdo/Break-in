#ifndef _GAME_ITEM_INCLUDE
#define _GAME_ITEM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class GameItem
{
	public:
		void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
		void update(int deltaTime);
		void render();
		
		void setTileMap(TileMap *tileMap);
		void setPosition(const glm::vec2 &pos);

		glm::ivec2 getPosition();

	private:
		glm::ivec2 tileMapDispl, pos;
		Texture spritesheet;
		Sprite *sprite;
		TileMap *map;
};
#endif