#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 36
#define FALL_STEP 4
#define INITIAL_POSITION

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, FALLING
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool mirror)
{
	this->mirror = mirror;
	
	if (this->mirror==true) spritesheet.loadFromFile("images/fireman_lite_inverse2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/fireman_lite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bJumping = false;
	
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.06666666667f,1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	
		sprite->setAnimationSpeed(STAND_LEFT, 16);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 16);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 4);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.06666666667f * 6.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.06666666667f * 8.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.06666666667f * 10.f, 0.f));

		
		sprite->setAnimationSpeed(MOVE_RIGHT, 9);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.06666666667f * 11.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.06666666667f * 12.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.06666666667f * 13.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.06666666667f * 14.f, 0.f));

		sprite->setAnimationSpeed(FALLING, 5);
		sprite->addKeyframe(FALLING, glm::vec2(0.06666666667f * 1.f, 0.f));
		sprite->addKeyframe(FALLING, glm::vec2(0.06666666667f * 2.f, 0.f));
		sprite->addKeyframe(FALLING, glm::vec2(0.06666666667f * 3.f, 0.f));
		sprite->addKeyframe(FALLING, glm::vec2(0.06666666667f * 4.f, 0.f));

		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		if (!this->mirror) {
			jumpAngle += JUMP_ANGLE_STEP;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) 
			{
				jumpAngle = 180;
			}
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		else {
			jumpAngle += JUMP_ANGLE_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				jumpAngle = 180;
			}
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY + JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		
	}
	else
	{
		if(!this->mirror) posPlayer.y += FALL_STEP;
		else  posPlayer.y -= FALL_STEP;
		
		if(!this->mirror) {
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				if (sprite->animation() == FALLING) {
					sprite->changeAnimation(STAND_RIGHT);
				}
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
			else {
				if (sprite->animation() != FALLING) {
					sprite->changeAnimation(FALLING);
				}
				if (posPlayer.y > 400) {
					posPlayer.x = 200;
					posPlayer.y = 100;
				}
			}
		}
		else {
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				if (sprite->animation() == FALLING) {
					sprite->changeAnimation(STAND_RIGHT);
				}
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
			else {
				if (sprite->animation() != FALLING) {
					sprite->changeAnimation(FALLING);
				}
				if (posPlayer.y < 200) {
					posPlayer.x = 200;
					posPlayer.y = 400;
				}
			}
		}
		
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
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

bool Player::isDead() {
	if (!this->mirror) {
		return posPlayer.y > 270;
	}
	else {
		return posPlayer.y < 270;
	}
}




