#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 2

glm::vec2 Player::checkColission(Ball* ball) // AABB - Circle collision
{
	//Classical way to do it
	/*bool xCollision = ball->posBall.x + 32.0f >= this->pos.x && this->pos.x + 32.0f >= ball->posBall.x;
	bool yCollision = ball->posBall.y + 32.0f >= this->pos.y && this->pos.y + 32.0f >= ball->posBall.y;*/

	//Obtaining center of the ball
	glm::vec2 center(ball->posBall + ball->radius);
	//Parameters needed for clamp function
	glm::vec2 aabbHalf(this->size.x / 2.0f, this->size.y / 2.0f);
	glm::vec2 aabbCenter(this->posPlayer.x + aabbHalf.x, this->posPlayer.y + aabbHalf.y);
	//Difference between both centers
	glm::vec2 difference = center - aabbCenter;
	//Clamped vector is the closest point from the AABB to the circle
	glm::vec2 clamped = glm::clamp(difference, -aabbHalf, aabbHalf);

	glm::vec2 closest = aabbCenter + clamped;
	//New difference, now using what we have calculated
	difference = closest - center;
	if (glm::length(difference) < ball->radius)
		int a = 0;

	return difference;
}

bool Player::checkBlockColission(Block * block)
{
	bool xCollision = posPlayer.x + ball->size.x >= block->pos.x && block->pos.x + size.x >= posPlayer.x;
	bool yCollision = posPlayer.y + ball->size.y >= block->pos.y && block->pos.y + size.y >= posPlayer.y;

	return xCollision && yCollision;
}

bool Player::checkBlockColissionWithBall(Ball* ball)
{
	bool xCollision = ball->posBall.x + ball->size.x >= this->posPlayer.x && this->posPlayer.x + size.x >= ball->posBall.x;
	bool yCollision = ball->posBall.y + ball->size.y >= this->posPlayer.y && this->posPlayer.y + size.y >= ball->posBall.y;

	return xCollision && yCollision;
}

Direction2 Player::detectCollisioningWithIndestructibleBlocks()
{
	for (int i = 0; i < indestructibleAreas.size(); i++) {
		glm::vec4 area = indestructibleAreas[i];
		float top = area.y - 8;
		float bot = area.w;
		float left = area.x;
		float right = area.z;

		if (posPlayer.y >= top && posPlayer.y <= bot && posPlayer.x + size.x >= left && posPlayer.x <= right) {
			float auxY = posPlayer.y;
			
			auxY += 5;
			if (auxY > bot)
				return UP;
			auxY = posPlayer.y;
			auxY -= 5;
			if (auxY < top)
				return DOWN;
			auxY = posPlayer.y;
		}

		if (posPlayer.y >= top && posPlayer.y <= bot) {
			float auxX = posPlayer.x;
			if (auxX >= left && auxX <= right) {
				return RIGHTSIDE;
			}
			auxX = posPlayer.x + size.x;
			if (auxX >= left && auxX <= right) {
				return LEFTSIDE;
			}
		}

	}

	return NONES;
}

Direction Player::getReboundDirection(glm::vec2 difference)
{
	Direction d = NONE;
	//We assign direction in function of thee side of the block colisioning with the ball.
	if (difference.y < 0)
		d = SOUTHEAST;
	else if (difference.y > 0)
		d = NORTHEAST;
	else if (difference.x > 0)
		d = EAST;
	else
		d = WEST;

	return d;
}

void Player::hookTheBall(Ball * ball)
{
	ball->posBall.x = posPlayer.x + (size.x / 2.0) - ball->size.x/2;
	ball->posBall.y = (posPlayer.y - size.y - ball->size.y/2 + 1);
}

enum PlayerAnimDirections {
	LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM, QUITE, CLOSED, TOP, BOTTOM, LEFT, RIGHT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/paddleanim3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	this->size = glm::ivec2(32.0, 8.0);
	sprite->setNumberAnimations(10);
	sprite->setAnimationSpeed(LEFTTOP, 10);
	sprite->setAnimationSpeed(RIGHTTOP, 10);
	sprite->setAnimationSpeed(LEFTBOTTOM, 10);
	sprite->setAnimationSpeed(CLOSED, 10);
	sprite->setAnimationSpeed(QUITE, 10);
	sprite->setAnimationSpeed(TOP, 10);
	sprite->setAnimationSpeed(BOTTOM, 10);
	sprite->setAnimationSpeed(RIGHTBOTTOM, 10);
	sprite->setAnimationSpeed(LEFT, 10);
	sprite->setAnimationSpeed(RIGHT, 10);

	sprite->addKeyframe(LEFTTOP, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(RIGHTTOP, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(LEFTBOTTOM, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(RIGHTBOTTOM, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(LEFT, glm::vec2(0.8f, 0.0f));

	sprite->addKeyframe(QUITE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(CLOSED, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(TOP, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(BOTTOM, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(RIGHT, glm::vec2(0.8f, 0.5f));

	sprite->changeAnimation(QUITE);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!isDead) {
		bool left, right, top, bot;
		left = right = top = bot = false;

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && detectCollisioningWithIndestructibleBlocks() != RIGHTSIDE)
		{
			posPlayer.x -= SPEED;
			sprite->changeAnimation(LEFT);
			left = true;
			if (map->collisionMoveLeft(posPlayer, size))
			{
				posPlayer.x += SPEED;
				sprite->changeAnimation(QUITE);
				left = false;
			}

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && detectCollisioningWithIndestructibleBlocks() != LEFTSIDE)
		{

			posPlayer.x += SPEED;
			sprite->changeAnimation(RIGHT);
			right = true;
			if (map->collisionMoveRight(posPlayer, size))
			{
				posPlayer.x -= SPEED;
				sprite->changeAnimation(QUITE);
				right = false;
			}
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && detectCollisioningWithIndestructibleBlocks() != UP)
		{
			posPlayer.y -= SPEED;
			top = true;

			if (right)
				sprite->changeAnimation(TOPLEFT);
			else if (left)
				sprite->changeAnimation(TOPRIGHT);
			else
				sprite->changeAnimation(TOP);

			if (map->collisionMoveTop(posPlayer, size)) {
				posPlayer.y += SPEED;
				top = false;
				sprite->changeAnimation(QUITE);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && posPlayer.y < 1016 && detectCollisioningWithIndestructibleBlocks() != DOWN)
		{
			posPlayer.y += SPEED;
			bot = true;

			if (right)
				sprite->changeAnimation(BOTTOMLEFT);
			else if (left)
				sprite->changeAnimation(BOTTOMRIGHT);
			else
				sprite->changeAnimation(BOTTOM);

			if (map->collisionMoveDown(posPlayer, size)) {
				posPlayer.y -= SPEED;
				bot = false;
				sprite->changeAnimation(QUITE);
			}
		}

		if (!right && !left && !top && !bot)
			sprite->changeAnimation(QUITE);

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

	//LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM, QUITE, CLOSED, TOP, BOTTOM, LEFT, RIGHT
	else {
		time += deltaTime;

		if (time < 700) {
			sprite->changeAnimation(TOP);
		}
		else if (time < 1400) {
			sprite->changeAnimation(LEFT);
		}
		else if (time < 2100) {
			sprite->changeAnimation(RIGHT);
		}
		else if (time < 2800) {
			sprite->changeAnimation(BOTTOM);
		}
		else if (time < 3500) {
			sprite->changeAnimation(QUITE);
		}
		else if (time < 4100){
			sprite->changeAnimation(CLOSED);
		}
		else {
			deadAnimEnded = true;
			isDead = false;
			time = 0;
		}
	}
}

void Player::render()
{
	sprite->render();
}

glm::ivec2 Player::getSizePlayer()
{
	return size;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::deadAnimation()
{
	isDead = true;
}




