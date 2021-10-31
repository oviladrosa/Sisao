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
	posObstacle = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObstacle.x), float(tileMapDispl.y + posObstacle.y)));
}

bool Box::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + posObstacle.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + posObstacle.y));
}

bool Box::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;

	x = float(posCollider.x + 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y - 1);
	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (y >= float(posObstacle.y) && y <= float(posObstacle.y + size.y) && x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x))
			return true;
	}
	return false;
}

bool Box::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;
	
	x = float(posCollider.x + size.x - 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y - 1);
	for (float y = y0; y <= y1; y += 0.1f)
	{
		if(y >= float(posObstacle.y) && y <= float(posObstacle.y + size.y) && x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x))
			return true;
	}
	return false;
}

bool Box::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;

	x0 = (posCollider.x + 10) / tileSize;
	x1 = (posCollider.x + size.x - 10) / tileSize;
	y = (posCollider.y) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (y * 64 + x == (posObstacle.y / 32) * 64 + (posObstacle.x / 32))
		{
			*posY = tileSize * y + size.y;
			return true;
		}
	}
	return false;
}
bool Box::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) const
{
	float x0, x1, y;
	int intY;

	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y + size.y);
	intY = (posCollider.y + size.y - 1) / 32;
	for (float x = x0; x <= x1; x+=0.1f)
	{
		if (x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x) && y >= float(posObstacle.y) && y <= float(posObstacle.y + size.y))
		{
			*posY = 32 * intY - size.y;
			return true;
		}

	}
	return false;
}
