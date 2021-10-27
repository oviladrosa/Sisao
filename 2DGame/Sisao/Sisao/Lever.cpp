#include "Lever.h"
#include "Sprite.h"
#include "TileMap.h"

enum LeverAnims
{
	DISABLED, ENABLED
};

Lever::Lever() : Obstacle()
{
}

Lever::~Lever()
{
}

void Lever::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Lever.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DISABLED, 1);
	sprite->addKeyframe(DISABLED, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
	enabled = false;
}

void Lever::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Lever::render()
{
	sprite->render();
}

void Lever::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Lever::setPosition(const glm::vec2& pos)
{
	initialPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool Lever::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + initialPosition.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + initialPosition.y));
}

bool Lever::isEnabled()
{
	return enabled;
}

void Lever::setEnabled(bool enable)
{
	enabled = enable;
}

bool Lever::LeftCollision(glm::vec2& posCollider)
{
	return false;
}
bool Lever::RightCollision(glm::vec2& posCollider)
{
	return false;
}
bool Lever::UpperCollision(glm::vec2& posCollider)
{
	return false;
}
bool Lever::BottomCollision(glm::vec2& posCollider)
{
	return false;
}