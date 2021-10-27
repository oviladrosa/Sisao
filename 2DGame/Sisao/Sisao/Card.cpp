#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Card.h"
#include "Game.h"

#define INITIAL_POSITION

enum CardAnims
{
	HOVER
};

void Card::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Card.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.125f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(HOVER, 8);
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 0.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 1.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 2.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 3.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 4.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 5.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 6.f, 0.f));
		sprite->addKeyframe(HOVER, glm::vec2(0.125f * 7.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void Card::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Card::render()
{
	sprite->render();
}

void Card::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Card::setPosition(const glm::vec2& pos)
{
	initialPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool Card::isPlayerTouching(glm::vec2& posPlayer) 
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + initialPosition.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + initialPosition.y));
}

