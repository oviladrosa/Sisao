#include "Box.h"
#include "Sprite.h"
#include "TileMap.h"

enum BoxAnims
{
	IDLE
};

Box::Box() : Obstacle()
{
}

Box::~Box()
{
}

void Box::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Box2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void Box::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Box::render()
{
	sprite->render();
}

void Box::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Box::setPosition(const glm::vec2& pos)
{
	initialPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool Box::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + initialPosition.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + initialPosition.y));
}

bool Box::LeftCollision(glm::vec2& posCollider)
{
	return false;
}
bool Box::RightCollision(glm::vec2& posCollider)
{
	return false;
}
bool Box::UpperCollision(glm::vec2& posCollider)
{
	return false;
}
bool Box::BottomCollision(glm::vec2& posCollider)
{
	return false;
}