#include "HydraulicPress.h"
#include "Sprite.h"
#include "TileMap.h"

enum HydraulicPressAnims
{
	PRESS_DOWN, PRESS_UP
};

HydraulicPress::HydraulicPress() : Obstacle()
{
}

HydraulicPress::~HydraulicPress()
{
}

void HydraulicPress::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Hammer.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(PRESS_DOWN, 8);
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 0.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 1.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 2.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 3.f, 0.f));
	
	sprite->setAnimationSpeed(PRESS_UP, 4);
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 0.f, 0.f));
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 1.f, 0.f));
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 2.f, 0.f));
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 3.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void HydraulicPress::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void HydraulicPress::render()
{
	sprite->render();
}

void HydraulicPress::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void HydraulicPress::setPosition(const glm::vec2& pos)
{
	initialPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool HydraulicPress::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + initialPosition.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + initialPosition.y));
}

bool HydraulicPress::LeftCollision(glm::vec2& posCollider) 
{
	return false; 
}
bool HydraulicPress::RightCollision(glm::vec2& posCollider) 
{ 
	return false; 
}
bool HydraulicPress::UpperCollision(glm::vec2& posCollider) 
{ 
	return false; 
}
bool HydraulicPress::BottomCollision(glm::vec2& posCollider) 
{ 
	return false; 
}