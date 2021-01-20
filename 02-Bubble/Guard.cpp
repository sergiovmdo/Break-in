#include "Guard.h"

void Guard::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	activated = true;
	spritesheet.loadFromFile("images/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::vec2(16, 16);
	pos = glm::ivec2(20, 250);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Guard::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (!catched) {
		if (objective->pos.y > pos.y)
			velocity.y = 0;
		if (objective->pos.x < pos.x + size.x)
			velocity.x = 0;

		if (velocity.y == 0 && velocity.x == 0)
			catched = true;

		pos += velocity;
	}

	else {
		pos += transportingVelocity;

		objective->setPosition(pos);

	}

	if (pos.x < 0 || pos.x > 418 || pos.y < 0 || pos.y > 330) {
		activated = false;
		objective->isDestroyed = true;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Guard::render()
{
	if (activated)
		sprite->render();
}

bool Guard::checkColission()
{
	bool xCollision = player->posPlayer.x + player->getSizePlayer().x >= this->pos.x && this->pos.x + size.x >= player->posPlayer.x;
	bool yCollision = player->posPlayer.y + player->getSizePlayer().y >= this->pos.y && this->pos.y + size.y >= player->posPlayer.y;

	return xCollision && yCollision;
}

void Guard::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Guard::setPosition(const glm::vec2 & pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Guard::removeBag()
{
	int dir = rand() % 3;

	if (objective == NULL) {
		activated = false;
		return;
	}

	if (objective->pos.x > 186) {
		switch (dir) {
		case 0:
			transportingVelocity = glm::ivec2(-1, 0);
		case 1:
			transportingVelocity = glm::ivec2(-1, 1);
			break;
		case 2:
			transportingVelocity = glm::ivec2(0, 1);
			break;

		}
	}
	else {
		switch (dir) {
		case 0:
			transportingVelocity = glm::ivec2(1, 0);
		case 1:
			transportingVelocity = glm::ivec2(1, 1);
			break;
		case 2:
			transportingVelocity = glm::ivec2(0, 1);
			break;

		}
	}
}
