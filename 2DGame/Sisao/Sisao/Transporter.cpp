#include "Transporter.h"
#include "Sprite.h"
#include "TileMap.h"

enum HydraulicPressAnims
{
	TRANSPORTING_RIGHT, TRANSPORTING_LEFT
};

Transporter::Transporter() : Obstacle()
{
}

Transporter::~Transporter()
{
}

void Transporter::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	left_direction = false;
	spritesheet.loadFromFile("images/Transporter_long.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(96, 32), glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(TRANSPORTING_RIGHT, 8);
	sprite->addKeyframe(TRANSPORTING_RIGHT, glm::vec2(0.25f * 0.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_RIGHT, glm::vec2(0.25f * 1.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_RIGHT, glm::vec2(0.25f * 2.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_RIGHT, glm::vec2(0.25f * 3.f, 0.f));

	sprite->setAnimationSpeed(TRANSPORTING_LEFT, 8);
	sprite->addKeyframe(TRANSPORTING_LEFT, glm::vec2(0.25f * 3.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_LEFT, glm::vec2(0.25f * 2.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_LEFT, glm::vec2(0.25f * 1.f, 0.f));
	sprite->addKeyframe(TRANSPORTING_LEFT, glm::vec2(0.25f * 0.f, 0.f));
	
	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void Transporter::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Transporter::render()
{
	sprite->render();
}

void Transporter::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Transporter::setPosition(const glm::vec2& pos)
{
	posObstacle = pos;
	sprite->setPosition(glm::vec2(pos.x, pos.y));
}

bool Transporter::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + posObstacle.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + posObstacle.y));
}

bool Transporter::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x = float(posCollider.x + 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y);

	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (y >= float(posObstacle.y) && y <= float(posObstacle.y+16) && x == float(posObstacle.x+96) ) return true;
	}
	return false;
}
bool Transporter::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x, y0, y1;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x = float(posCollider.x + size.x - 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y);

	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (y >= float(posObstacle.y) && y <= float(posObstacle.y + 15) && x == float(posObstacle.x)) return true;
	}
	return false;
}
bool Transporter::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x0, x1, y;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y);

	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x) && x <= (float(posObstacle.x) + 96.f) && y == float(posObstacle.y+16)) return true;
	}
	return false;

}
bool Transporter::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const
{
	float x0, x1, y;
	int tileSize = map->getTileSize();
	if (tileSize == 0) tileSize = 32;


	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y + size.y);

	for (float x = x0; x <= x1; x+= 0.1f)
	{
		if (x >= float(posObstacle.x) && x <= (float(posObstacle.x) + 96.f) && y == float(posObstacle.y)) return true;
	}
	return false;
}

void Transporter::setDirection(bool dir) {
	left_direction = dir;
	if (left_direction) {
		sprite->changeAnimation(1);
	}
	else {
		sprite->changeAnimation(0);
	}
}

int Transporter::getMovement() {
	if (left_direction) {
		return -1;
	}
	else {
		return +1;
	}
}