#include "Putin.h"

void Putin::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/police.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::vec2(16, 16);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Putin::update(int deltaTime)
{
	sprite->update(deltaTime);

	float x = player->posPlayer.x;
	float y = player->posPlayer.y;

	bool left = false, right = false, bottom = false, top = false;

	Quarter playerQuarter = getPlayerQuarter();
	Quarter putinQuarter = getPutinQuarter();

	if (pos.y > player->posPlayer.y) {
		if (velocity.y > 0 && pos.y - size.y > player->posPlayer.y)
			velocity.y = -velocity.y;
		top = true;
	}
	else {
		if (velocity.y < 0 && pos.y < player->posPlayer.y - player->getSizePlayer().y)
			velocity.y = -velocity.y;
		bottom = true;
	}
	if (pos.x + size.x < player->posPlayer.x) {
		if (velocity.x < 0)
			velocity.x = -velocity.x;
		right = true;
	}
	else if (pos.x > player->posPlayer.x){
		if (velocity.x > 0)
			velocity.x = -velocity.x;
		left = true;
	}

	if (left) {
		if (pos.x + velocity.x > 20)
			pos.x += velocity.x;
	}
	if (right) {
		if (pos.x + velocity.x < 388)
			pos.x += velocity.x;
	}
	if (bottom && (left || right)) {
		if (sceneNumber == 3) {
			if (pos.y + velocity.y < 1016)
				pos.y += velocity.y;
		}
		else {
			if (pos.y + velocity.y < 306)
				pos.y += velocity.y;
		}
	}
	if (top && (left || right)) {
		if (sceneNumber == 3) {
			if (pos.y + velocity.y > 720)
				pos.y += velocity.y;
		}
		else {
			if (pos.y + velocity.y > 12)
				pos.y += velocity.y;
		}
	}

	/*Siempre mira si va en dirección del jugador, por lo tanto cuando el jugador le sobrepasa en cualquier
	eje el monigote cambia de dirección y se va a por el bote */

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Putin::render()
{
	if (enabled)
		sprite->render();
}

bool Putin::checkColission()
{
	bool xCollision = player->posPlayer.x + player->getSizePlayer().x >= this->pos.x && this->pos.x + size.x >= player->posPlayer.x;
	bool yCollision = player->posPlayer.y + player->getSizePlayer().y >= this->pos.y && this->pos.y + size.y >= player->posPlayer.y;

	return xCollision && yCollision;
}

void Putin::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Putin::setPosition(const glm::vec2 & pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

Quarter Putin::getPlayerQuarter()
{
	if (player->posPlayer.x > 186) {
		if (player->posPlayer.y < 149.5)
			return TOPRIGHT;
		else
			return BOTTOMRIGHT;
	}
	else {
		if (player->posPlayer.y < 149.5)
			return TOPLEFT;
		else
			return BOTTOMLEFT;
	}
}

Quarter Putin::getPutinQuarter()
{
	if (pos.x > 186) {
		if (pos.y < 149.5)
			return TOPRIGHT;
		else
			return BOTTOMRIGHT;
	}
	else {
		if (pos.y < 149.5)
			return TOPLEFT;
		else
			return BOTTOMLEFT;
	}
}
