#include "Ball.h"
#include "GameItem.h"
#include <iostream>

#include "Audio.h"

void Ball::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	hooked = true;
	spritesheet.loadFromFile("images/sphere.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(14, 14), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
	float radius = 9.9;
	size = glm::ivec2(14, 14);

	velocity = glm::ivec2(-1.0f, -1.0f);
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2 & pos)
{
	if (!hooked) {
		this->posBall = pos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	}
	else {
		this->posBall.x = pos.x + posPlayer.x;
		this->posBall.y = pos.y + posPlayer.y;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	}
}

void Ball::setPositionPlayer(const glm::vec2 & pos)
{
	this->posPlayer = pos;
}

void Ball::reset(glm::vec2 playerPosition)
{
	this->posBall = playerPosition;
	velocity.x = 0;
	velocity.y = 0;
	hooked = true;
}

void Ball::deleteBrick()
{
	bricksMap->destroyAtLeftCollision(this->posBall, glm::ivec2(32, 32));
	bricksMap->destroyAtRightCollision(this->posBall, glm::ivec2(32, 32));
	bricksMap->destroyAtTopCollision(this->posBall, glm::ivec2(32, 32));
	bricksMap->destroyAtDownCollision(this->posBall, glm::ivec2(32, 32));
}

void Ball::updateSprite()
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::looseLife()
{
	if (scene1) {
		if (posBall.x >= 140.0 && posBall.x <= 260.0f && posBall.y >= 1012.0) {
			return true;
		}
	}
	else if (scene2) {
		if (posBall.x >= 108.0 && posBall.x <= 292.0f && posBall.y >= 1012.0) {
			return true;
		}
	}
	else if (scene3) {
		if (posBall.x >= 76.0 && posBall.x <= 324.0f && posBall.y >= 1012.0) {
			return true;
		}
	}
	return false;
}

void Ball::addColisionAreas(glm::ivec2 area, glm::ivec2 size)
{
	pair<glm::ivec2 , glm::ivec2> p;
	p.first = area;
	p.second = size;
	colisionAreas.push_back(p);
}

void Ball::detectCollisioningWithIndestructibleBlocks()
{
	for (int i = 0; i < colisionAreas.size(); i++) {
		pair<glm::ivec2, glm::ivec2> p = colisionAreas[i];
		float x = p.first.x;
		float y = p.first.y;
		float width = p.second.y;
		float height = p.second.x;

		//La bola viene de debajo
		if (posBall.y > y + height) {
			Audio::instance().play(CHOQUE, false);
			velocity.y = -velocity.y;
		}
		//La bola viene de arriba
		else if (posBall.y < y) {
			Audio::instance().play(CHOQUE, false);
			velocity.y = -velocity.y;
		}
		if (posBall.y < y + height && posBall.y > y) {
			Audio::instance().play(CHOQUE, false);
			velocity.x = -velocity.x;
		}
	}

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	// if not stuck to player board

	// check if outside window bounds; if so, reverse velocity and restore at correct position
	this->posBall += velocity;
	if (this->posBall.x <= 15.0f)
	{
		Audio::instance().play(CHOQUE, false);
		this->velocity.x = -velocity.x;
		this->posBall.x = 15.0f;
	}
	else if (this->posBall.x + 32.0f >= 415.0f)
	{
		Audio::instance().play(CHOQUE, false);
		this->velocity.x = -velocity.x;
		this->posBall.x = 415 - 32.0f;
	}

	if (level1) {
		if (this->posBall.y <= 718.0)
		{
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 718.0f;
		}

		else if (this->posBall.y >= 1013.0) {
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 1013.0f;
		}

	}
	else if (level2) {

		if (this->posBall.y <= 365.0f)
		{
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 365.0f;
		}
		else if (this->posBall.y >= 659.0f) {
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 655.0f;
		}

	}
	else if (level3) {
		if (this->posBall.y <= 13.0f)
		{
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 13.0f;
		}
		else if (this->posBall.y >= 305) {
			Audio::instance().play(CHOQUE, false);
			this->velocity.y = -velocity.y;
			this->posBall.y = 300.0f;
		}

	}

	//detectCollisioningWithIndestructibleBlocks();

	//this->deleteBrick();
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

