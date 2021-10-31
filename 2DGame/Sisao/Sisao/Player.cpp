#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include <irrKlang.h>
using namespace irrklang;


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 36
#define FALL_STEP 4
#define INITIAL_POSITION

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, FALLING_RIGHT, FALLING_LEFT, JUMP_RIGHT_1, JUMP_RIGHT_2, JUMP_RIGHT_3, JUMP_LEFT_1, JUMP_LEFT_2, JUMP_LEFT_3, IDLE_LEFT, IDLE_RIGHT, FULL_JUMP
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool mirror)
{
	this->mirror = mirror;
	
	if (this->mirror==true) spritesheet.loadFromFile("images/fireman_total_inverse.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/fireman_total.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bJumping = false;
	float xsprites = 36.f;
	float ysprites = 2.f;
	float rx = 1.f / xsprites;
	float ry = 1.f / ysprites;
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(rx,ry), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(15);
	
		sprite->setAnimationSpeed(STAND_LEFT, 16);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(rx * 35.f, 0.5f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 16);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.f));

		
		sprite->setAnimationSpeed(MOVE_LEFT,16);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(rx * 31.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(rx * 30.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(rx * 29.f, 0.5f));

		
		sprite->setAnimationSpeed(MOVE_RIGHT, 16);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(rx * 4.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(rx * 5.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(rx * 6.f, 0.f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 5);
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(rx * 7.f, 0.f));
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(rx * 8.f, 0.f));
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(rx * 9.f, 0.f));
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(rx * 10.f, 0.f));

		sprite->setAnimationSpeed(FALLING_LEFT, 5);
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(rx * 28.f, 0.5f));
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(rx * 27.f, 0.5f));
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(rx * 26.f, 0.5f));
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(rx * 25.f, 0.5f));

		sprite->setAnimationSpeed(JUMP_RIGHT_1, 5);
		sprite->addKeyframe(JUMP_RIGHT_1, glm::vec2(rx * 1.f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT_2, 5);
		sprite->addKeyframe(JUMP_RIGHT_2, glm::vec2(rx * 2.f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT_3, 5);
		sprite->addKeyframe(JUMP_RIGHT_3, glm::vec2(rx * 17.f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT_1, 5);
		sprite->addKeyframe(JUMP_LEFT_1, glm::vec2(rx * 34.f, 0.5f));

		sprite->setAnimationSpeed(JUMP_LEFT_2, 5);
		sprite->addKeyframe(JUMP_LEFT_2, glm::vec2(rx * 33.f, 0.5f));

		sprite->setAnimationSpeed(JUMP_LEFT_3, 5);
		sprite->addKeyframe(JUMP_LEFT_3, glm::vec2(rx * 18.f, 0.5f));

		sprite->setAnimationSpeed(IDLE_LEFT, 5);

		sprite->setAnimationSpeed(IDLE_RIGHT, 5);
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(rx * 14.f, 0.f));
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(rx * 15.f, 0.f));
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(rx * 16.f, 0.f));

		sprite->setAnimationSpeed(IDLE_LEFT, 5);
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(rx * 21.f, 0.5f));
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(rx * 20.f, 0.5f));
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(rx * 19.f, 0.5f));

		sprite->setAnimationSpeed(FULL_JUMP, 5);
		sprite->addKeyframe(FULL_JUMP, glm::vec2(rx * 1.f, 0.f));
		sprite->addKeyframe(FULL_JUMP, glm::vec2(rx * 2.f, 0.f));
		sprite->addKeyframe(FULL_JUMP, glm::vec2(rx * 17.f, 0.f));
		


		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	lastMove = 0;
	demo = false;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!demo) {

	
		if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			lastMove = 0;
			if(sprite->animation() != MOVE_LEFT && !bJumping)
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
			lastMove = 0;
			if(sprite->animation() != MOVE_RIGHT && !bJumping)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (lastMove > 100) {
		
			if (sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(IDLE_LEFT);
			}
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(IDLE_RIGHT);
			}
		}
		else
		{
			lastMove++;
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
				
				if (jumpAngle == 44) {
					if (sprite->animation() == JUMP_LEFT_1) {
						sprite->changeAnimation(JUMP_LEFT_2);
					}
					else {
						sprite->changeAnimation(JUMP_RIGHT_2);
					}
				}
				if (jumpAngle == 92) {
					if (sprite->animation() == JUMP_LEFT_2) {
						sprite->changeAnimation(JUMP_LEFT_3);
					}
					else {
						sprite->changeAnimation(JUMP_RIGHT_3);
					}
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
				if (jumpAngle == 44) {
					if (sprite->animation() == JUMP_LEFT_1) {
						sprite->changeAnimation(JUMP_LEFT_2);
					}
					else {
						sprite->changeAnimation(JUMP_RIGHT_2);
					}
				}
				if (jumpAngle == 92) {
					if (sprite->animation() == JUMP_LEFT_2) {
						sprite->changeAnimation(JUMP_LEFT_3);
					}
					else {
						sprite->changeAnimation(JUMP_RIGHT_3);
					}
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
					if (sprite->animation() == FALLING_RIGHT || sprite->animation() == JUMP_RIGHT_1 || sprite->animation() == JUMP_RIGHT_2 || sprite->animation() == JUMP_RIGHT_3) {
						sprite->changeAnimation(STAND_RIGHT);
					}
					else if (sprite->animation() == FALLING_LEFT || sprite->animation() == JUMP_LEFT_1 || sprite->animation() == JUMP_LEFT_2 || sprite->animation() == JUMP_LEFT_3) {
						sprite->changeAnimation(STAND_LEFT);
					}
					if (Game::instance().getSpecialKey(GLUT_KEY_UP))
					{
						bJumping = true;
						jumpAngle = 0;
						startY = posPlayer.y;
						lastMove = 0;
					
						if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == FALLING_LEFT || sprite->animation() == IDLE_LEFT) {
							sprite->changeAnimation(JUMP_LEFT_1);
						}
						else {
							sprite->changeAnimation(JUMP_RIGHT_1);
						}
						SoundEnginePlayer->play2D("audio/jump-sound.wav", false);
					}
				}
				else {
					if (sprite->animation() != FALLING_RIGHT && (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT_1 || sprite->animation() == JUMP_RIGHT_2 || sprite->animation() == JUMP_RIGHT_3)) {
						sprite->changeAnimation(FALLING_RIGHT);
					}
					if (sprite->animation() != FALLING_LEFT && (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT_1 || sprite->animation() == JUMP_LEFT_2 || sprite->animation() == JUMP_LEFT_3)) {
						sprite->changeAnimation(FALLING_LEFT);
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
					if (sprite->animation() == FALLING_RIGHT || sprite->animation() == JUMP_RIGHT_1 || sprite->animation() == JUMP_RIGHT_2 || sprite->animation() == JUMP_RIGHT_3) {
						sprite->changeAnimation(STAND_RIGHT);
					}
					else if (sprite->animation() == FALLING_LEFT || sprite->animation() == JUMP_LEFT_1 || sprite->animation() == JUMP_LEFT_2 || sprite->animation() == JUMP_LEFT_3) {
						sprite->changeAnimation(STAND_LEFT);
					}
					if (Game::instance().getSpecialKey(GLUT_KEY_UP))
					{
						bJumping = true;
						jumpAngle = 0;
						startY = posPlayer.y;
						lastMove = 0;
						if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == FALLING_LEFT || sprite->animation() == IDLE_LEFT) {
							sprite->changeAnimation(JUMP_LEFT_1);
						}
						else {
							sprite->changeAnimation(JUMP_RIGHT_1);
						}
					}
				}
				else {
					if (sprite->animation() != FALLING_RIGHT && (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)) {
						sprite->changeAnimation(FALLING_RIGHT);
					}
					if (sprite->animation() != FALLING_LEFT && (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)) {
						sprite->changeAnimation(FALLING_LEFT);
					}
					if (posPlayer.y < 200) {
						posPlayer.x = 200;
						posPlayer.y = 400;
					}
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


void Player::setSoundEngine(ISoundEngine *SoundEngine) {
	SoundEnginePlayer = SoundEngine;
}

bool Player::isDead() {
	if (!this->mirror) {
		return posPlayer.y > 220;
	}
	else {
		return posPlayer.y < 270;
	}
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

bool Player::isMirror()
{
	return mirror;
}

bool Player::isJumping()
{
	return bJumping;
}

void Player::changeDemo(bool d) {
	demo = d;
}

void Player::forceAnimation(int a) {
	sprite->changeAnimation(a);
}




