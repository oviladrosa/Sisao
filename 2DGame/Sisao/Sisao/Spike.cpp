#include "Spike.h"
#include "Sprite.h"
#include "TileMap.h"

enum SpikeAnims
{
	SPIKE_DEFAULT
};

Spike::Spike() : Obstacle()
{
}

Spike::~Spike()
{
}

void Spike::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	if (!mirror) {
		spritesheet.loadFromFile("images/Spike.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	else {
		spritesheet.loadFromFile("images/upside_spike.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(SPIKE_DEFAULT, 8);
	sprite->addKeyframe(SPIKE_DEFAULT, glm::vec2(0.f, 0.f));
	


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void Spike::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Spike::render()
{
	sprite->render();
}

void Spike::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Spike::setPosition(const glm::vec2& pos)
{
	posObstacle = pos;
	sprite->setPosition(glm::vec2(pos.x, pos.y));
}

bool Spike::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + posObstacle.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + posObstacle.y));
}

bool Spike::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x = float(posCollider.x + 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y-10);

	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (!mirror && y >= float(posObstacle.y + 10) && y <= float(posObstacle.y + 32) && x == float(posObstacle.x+32)) return true;
		if (mirror && y >= float(posObstacle.y) && y <= float(posObstacle.y + 18) && x == float(posObstacle.x+32)) return true;

	}
	return false;
}
bool Spike::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x = float(posCollider.x + size.x - 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y -10);

	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (!mirror && y >= float(posObstacle.y+10) && y <= float(posObstacle.y + 32) && x == float(posObstacle.x)) return true;
		if (mirror && y >= float(posObstacle.y) && y <= float(posObstacle.y + 18) && x == float(posObstacle.x)) return true;

	}
	return false;
}
bool Spike::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x0, x1, y;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y);

	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x+4) && x <= (float(posObstacle.x) + 28.f) && y >= float(posObstacle.y) && y <= float(posObstacle.y+20)) return true;
	}
	return false;

}
bool Spike::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x0, x1, y;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;

	
	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y + size.y);

	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x+4) && x <= (float(posObstacle.x) + 28.f) && y == float(posObstacle.y+22)) return true;
	}
	return false;
	
	
}

void Spike::setSpike(bool mirr) {
	mirror = mirr;
}

