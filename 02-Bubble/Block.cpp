#include "Block.h"

glm::vec2 Block::checkColission(Ball* ball) // AABB - Circle collision
{
	/* Classical way to do it
	bool xCollision = ball->posBall.x + 32.0f >= this->pos.x && this->pos.x + 32.0f >= ball->posBall.x;
	bool yCollision = ball->posBall.y + 32.0f >= this->pos.y && this->pos.y + 32.0f >= ball->posBall.y;*/

	//Obtaining center of the ball
	glm::vec2 center(ball->posBall + ball->radius);
	//Parameters needed for clamp function
	glm::vec2 aabbHalf(this->size.x / 2.0f, this->size.x / 2.0f);
	glm::vec2 aabbCenter(this->pos.x + aabbHalf.x, this->pos.y + aabbHalf.y);
	//Difference between both centers
	glm::vec2 difference = center - aabbCenter;
	//Clamped vector is the closest point from the AABB to the circle
	glm::vec2 clamped = glm::clamp(difference, -aabbHalf, aabbHalf);
	
	glm::vec2 closest = aabbCenter + clamped;
	//New difference, now using what we have calculated
	difference = closest - center;

	return difference;
}

bool Block::checkColission2(Ball * ball)
{
	bool xCollision = ball->posBall.x + ball->size.x >= this->pos.x && this->pos.x + size.x >= ball->posBall.x;
	bool yCollision = ball->posBall.y + ball->size.y >= this->pos.y && this->pos.y + size.y >= ball->posBall.y;

	return xCollision && yCollision;
}

bool Block::checkColissionWithGuard(glm::ivec2 pos, glm::ivec2 size)
{
	bool xCollision = pos.x + size.x >= this->pos.x && this->pos.x + size.x >= pos.x;
	bool yCollision = pos.y + size.y >= this->pos.y && this->pos.y + size.y >= pos.y;

	return xCollision && yCollision;
}

bool Block::checkTopColission(glm::ivec2 pos, glm::ivec2 size)
{
	return this->pos.y <= pos.y - size.y;
}

bool Block::checkBotColission(glm::ivec2 pos, glm::ivec2 size)
{
	return this->pos.y - this->size.y <= pos.y;
}

bool Block::checkRightColission(glm::ivec2 pos, glm::ivec2 size)
{
	return this->pos.x < pos.x;
}

bool Block::checkLeftColission(glm::ivec2 pos, glm::ivec2 size)
{
	return this->pos.x > pos.x + size.x;
}

Direction Block::getReboundDirection(glm::vec2 difference)
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

Direction Block::getReboundDirection2(Ball *ball)
{
	Direction d = NONE;


	if (pos.y > ball->posBall.y) {
		if (pos.x + size.x > ball->posBall.x + ball->radius) {
			return NORTHWEST;
		}
		else {
			return NORTHEAST;
		}
	}
	else {
		if (pos.x + size.x > ball->posBall.x + ball->radius) {
			return SOUTHWEST;
		}
		else {
			return SOUTHEAST;
		}
	}

	//Unused
	if (pos.x > ball->posBall.x) {
		d = EAST;
	}
	else if (pos.x + size.x < ball->posBall.x + 10) {
		d = WEST;
	}

	return d;
}


void Block::setPosition(const glm::vec2 & pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::reset(glm::vec2 playerPosition)
{
	
}

enum BlockTypes
{
	HOPE_ITS_BLUE, HOPE_ITS_BROWN, HOPE_ITS_RED, HOPE_ITS_GREY, HOPE_ITS_PURPLE, HOPE_ITS_DARK_GREY, HOPE_ITS_PENULTIMO, HOPE_ITS_ULTIMO
};

void Block::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/test3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	size = glm::vec2(32, 16);
	sprite->setNumberAnimations(8);

		sprite->addKeyframe(HOPE_ITS_BLUE, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(HOPE_ITS_BROWN, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(HOPE_ITS_RED, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(HOPE_ITS_GREY, glm::vec2(0.75f, 0.f));
		sprite->addKeyframe(HOPE_ITS_PURPLE, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(HOPE_ITS_DARK_GREY, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(HOPE_ITS_PENULTIMO, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(HOPE_ITS_ULTIMO, glm::vec2(0.75f, 0.5f));

}

void Block::initGateBlocks(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isGateBlock = true;
	spritesheet.loadFromFile("images/test.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setNumberAnimations(1);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initGateBlocks2(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isGateBlock = true;
	spritesheet.loadFromFile("images/tiles2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initGateBlocks3(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isGateBlock = true;
	spritesheet.loadFromFile("images/tiles3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initIndestructibleBlocks(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isIndestructible = true;
	spritesheet.loadFromFile("images/test.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(16, 16);
	sprite->setNumberAnimations(1);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initIndestructibleBlocks2(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isIndestructible = true;
	spritesheet.loadFromFile("images/tiles2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(16, 16);
	sprite->setNumberAnimations(1);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initIndestructibleBlocks3(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isIndestructible = true;
	spritesheet.loadFromFile("images/tiles3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(16, 16);
	sprite->setNumberAnimations(1);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initKeyBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isKey = true;
	spritesheet.loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::vec2(32.0, 32.0);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initBagBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/money-bag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(24, 24);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initCoinBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/dollar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(24, 24);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initAlarmBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isAlarm = true;
	spritesheet.loadFromFile("images/alarms.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.5, 1.0), &spritesheet, &shaderProgram);
	size = glm::ivec2(24,24);
	tileMapDispl = tileMapPos;

	sprite->setNumberAnimations(2);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initLifeBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isLife = true;
	spritesheet.loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(32, 32);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initDiamondBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/diamond.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	size = glm::ivec2(24, 24);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initGuardActivationBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isGuardianBlock = true;
	spritesheet.loadFromFile("images/alarms2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.5, 1.0), &spritesheet, &shaderProgram);
	size = glm::ivec2(24, 24);
	tileMapDispl = tileMapPos;

	sprite->setNumberAnimations(2);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::initPutinAnimBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isPutinAnim = true;
	spritesheet.loadFromFile("images/putin_explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1.0/3.0, 1.0 / 3.0), &spritesheet, &shaderProgram);
	size = glm::ivec2(24, 24);
	tileMapDispl = tileMapPos;

	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(0, 10);
	sprite->setAnimationSpeed(1, 10);
	sprite->setAnimationSpeed(2, 10);
	sprite->setAnimationSpeed(3, 10);
	sprite->setAnimationSpeed(4, 10);
	sprite->setAnimationSpeed(5, 10);
	sprite->setAnimationSpeed(6, 10);
	sprite->setAnimationSpeed(7, 10);
	sprite->setAnimationSpeed(8, 10);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(1.0 / 3.0, 0.f));
	sprite->addKeyframe(2, glm::vec2(2.0 / 3.0, 0.f));
	sprite->addKeyframe(3, glm::vec2(0.f , 1.0 / 3.0));
	sprite->addKeyframe(4, glm::vec2(1.0 / 3.0, 1.0 / 3.0));
	sprite->addKeyframe(5, glm::vec2(2.0 / 3.0, 1.0 / 3.0));
	sprite->addKeyframe(6, glm::vec2(0.f , 2.0 / 3.0));
	sprite->addKeyframe(7, glm::vec2(1.0 / 3.0, 2.0 / 3.0));
	sprite->addKeyframe(8, glm::vec2(2.0 / 3.0, 2.0 / 3.0));

	sprite->changeAnimation(0);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}


void Block::setBlockNumber(int blockNumber)
{
	sprite->changeAnimation(blockNumber);
}

void Block::makeAnimation()
{
	if (animCountDown > 0)
		pos.y += 5;
	else if(animCountDown == 0)
		isDestroyed = true;
	
	--animCountDown;
}

void Block::deleteBlock()
{
	--hits;
	if (isMultiHit) {
		if (hits == 0) {
			touched = true;;
		}
		else if (hits == 2){
			sprite->changeAnimation(1);
		} 
		else {
			sprite->changeAnimation(2);
		}
	}
	else if (isDoubleHit) {
		if (hits == 0) {
			touched = true;
		}
		else if (hits == 1) {
			sprite->changeAnimation(5);
		}
	}
	else {
		touched = true;
		if (isGateBlock)
			isDestroyed = true;
	}
}


void Block::initMultiHitBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isMultiHit = true;
	spritesheet.loadFromFile("images/multihit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	size = glm::vec2(32, 16);
	sprite->setNumberAnimations(8);
	
	sprite->addKeyframe(HOPE_ITS_BLUE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(HOPE_ITS_BROWN, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(HOPE_ITS_RED, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(HOPE_ITS_GREY, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(HOPE_ITS_PURPLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_DARK_GREY, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_PENULTIMO, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_ULTIMO, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(0);
}

void Block::initDoubleHitBlock(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	isDoubleHit = true;
	hits = 2;
	spritesheet.loadFromFile("images/multihit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	size = glm::vec2(32, 16);
	sprite->setNumberAnimations(8);

	sprite->addKeyframe(HOPE_ITS_BLUE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(HOPE_ITS_BROWN, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(HOPE_ITS_RED, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(HOPE_ITS_GREY, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(HOPE_ITS_PURPLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_DARK_GREY, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_PENULTIMO, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(HOPE_ITS_ULTIMO, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(4);
}

void Block::initPortalPart1(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/portal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	size = glm::ivec2(24, 24);
	tileMapDispl = tileMapPos;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::update(int deltaTime)
{
	if (touched)
		makeAnimation();
	if (isDestroyed)
		sprite->setPosition(glm::ivec2(0, 0));
	if (isPutinAnim) {
		updatePutinAnim(deltaTime);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::render()
{
	if (!isDestroyed)
		sprite->render();
}

void Block::updatePutinAnim(int deltaTime)
{
		time += deltaTime;

		if (time < 200) {
			sprite->changeAnimation(1);
		}
		else if (time < 400) {
			sprite->changeAnimation(2);
		}
		else if (time < 600) {
			sprite->changeAnimation(3);
		}
		else if (time < 800) {
			sprite->changeAnimation(4);
		}
		else if (time < 1000) {
			sprite->changeAnimation(5);
		}
		else if (time < 1200) {
			sprite->changeAnimation(6);
		}
		else if (time < 1400) {
			sprite->changeAnimation(7);
		}
		else if (time < 1600) {
			sprite->changeAnimation(8);
		}
		else {
			putinAnimFinished = true;
			isDestroyed = true;
			isPutinAnim = false;
		}
	
}

void Block::activateButton()
{
	if (!buttonPressed) {
		this->size.y = this->size.y - 10;
		buttonPressed = true;
	}
	sprite->changeAnimation(1);
	
}
