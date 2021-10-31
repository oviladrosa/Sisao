#include "Wall.h"
#include "Sprite.h"
#include "TileMap.h"

enum WallAnims
{
	IDLE
};

Wall::Wall() : Obstacle()
{
}

Wall::~Wall()
{
}

void Wall::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Wall.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
	isActive = true;
}

void Wall::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Wall::render()
{
	sprite->render();
}

void Wall::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Wall::setPosition(const glm::vec2& pos)
{
	initialPosition = pos;
	posObstacle = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool Wall::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + initialPosition.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + initialPosition.y));
}

bool Wall::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	int x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;

	x = (posCollider.x + 10) / tileSize;
	y0 = posCollider.y / tileSize;
	y1 = (posCollider.y + size.y - 1) / tileSize;
	int pp = y0 * 64 + x;
	for (int y = y0; y <= y1; y++)
	{
		if (y * 64 + x == (posObstacle.y / 32) * 64 + (posObstacle.x / 32))
			return true;
	}
	return false;
}

bool Wall::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	int x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;

	x = (posCollider.x + size.x - 10) / tileSize;
	y0 = posCollider.y / tileSize;
	y1 = (posCollider.y + size.y - 1) / tileSize;
	int pp = y0 * 64 + x;
	for (int y = y0; y <= y1; y++)
	{
		if (y * 64 + x == (posObstacle.y / 32) * 64 + (posObstacle.x / 32))
			return true;
	}
	return false;
}

bool Wall::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	return false;
}
bool Wall::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	return false;
}

bool Wall::isWallActive()
{
	return isActive;
}

void Wall::setIsActive(bool active)
{
	isActive = active;
}