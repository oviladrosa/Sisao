#include "Box.h"
#include "Sprite.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define FALL_STEP 4

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
	collisioning = false;

	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

void Box::update(int deltaTime)
{
	sprite->update(deltaTime);
	this->collisioning = false;
	if (!this->mirror) posObstacle.y += FALL_STEP;
	else  posObstacle.y -= FALL_STEP;

	if (map->collisionObstacleMoveLeft(posObstacle, glm::ivec2(32, 32)))
	{
		posObstacle.x += 3;
		collisioning = true;
	}
	if (map->collisionObstacleMoveRight(posObstacle, glm::ivec2(32, 32)))
	{
		posObstacle.x -= 3;
		collisioning = true;
	}
	
	if (falling)
	{
		if (!this->mirror) {
			collisionAngle += JUMP_ANGLE_STEP;
			if (map->collisionObstacleMoveUp(posObstacle, glm::ivec2(32, 32), &posObstacle.y))
			{
				collisionAngle = 90;
			}
			if (collisionAngle >= 90)
			{
				falling = false;
				posObstacle.y = startY;
			}
			else
			{
				if (collisionAngle > 45)
					falling = !map->collisionObstacleMoveDown(posObstacle, glm::ivec2(32, 32), &posObstacle.y);
			}
		}
		else {
			collisionAngle += JUMP_ANGLE_STEP;
			if (map->collisionObstacleMoveDown(posObstacle, glm::ivec2(32, 32), &posObstacle.y))
			{
				collisionAngle = 90;
			}
			if (collisionAngle >= 90)
			{
				falling = false;
				posObstacle.y = startY;
			}
			else
			{
				if (collisionAngle > 45)
					falling = !map->collisionObstacleMoveUp(posObstacle, glm::ivec2(32, 32), &posObstacle.y);
			}
		}
	}
	else
	{
		if (!this->mirror) {
			if (!map->collisionObstacleMoveDown(posObstacle, glm::ivec2(32, 32), &posObstacle.y))
			{
				falling = true;
				collisionAngle = 0;
				startY = posObstacle.y;
			}
			if (posObstacle.y > 260) {
				posObstacle.y = 256;
			}
		}
		else {
			if (!map->collisionObstacleMoveUp(posObstacle, glm::ivec2(32, 32), &posObstacle.y))
			{
				falling = true;
				collisionAngle = 0;
				startY = posObstacle.y;
			}
			if (posObstacle.y < 284) {
				posObstacle.y = 288;
			}
		}
	}
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObstacle.x), float(tileMapDispl.y + posObstacle.y)));
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

bool Box::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size)
{
	float x, y0, y1;

	x = float(posCollider.x + 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y - 1);
	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (y >= float(posObstacle.y) && y <= float(posObstacle.y + size.y) && x >= float(posObstacle.x + size.x - 4) && x <= float(posObstacle.x + size.x)) 
		{
			return true;
		}
	}
	return false;
}

bool Box::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size)
{
	float x, y0, y1;
	
	x = float(posCollider.x + size.x - 10);
	y0 = float(posCollider.y);
	y1 = float(posCollider.y + size.y - 1);
	for (float y = y0; y <= y1; y += 0.1f)
	{
		if (y >= float(posObstacle.y) && y <= float(posObstacle.y + size.y) && x >= float(posObstacle.x) && x <= float(posObstacle.x + 4))
		{
			return true;
		}
	}
	return false;
}

bool Box::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY)
{
	float x0, x1, y;
	int intY;

	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y);
	intY = (posCollider.y + size.y - 10) / 32;
	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x) && y == float(posObstacle.y + size.y))
		{
			*posY = posCollider.y + 4;
			return true;
		}
	}
	return false;
}

bool Box::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY)
{
	float x0, x1, y;
	int intY;

	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y + size.y);
	intY = (posCollider.y + size.y - 10) / 32;
	for (float x = x0; x <= x1; x+=0.1f)
	{
		if (x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x) && y == float(posObstacle.y))
		{
			*posY = posCollider.y - 4;
			return true;
		}
	}
	return false;
}

bool Box::isMirror()
{
	return mirror;
}

void Box::setMirror(bool isBoxMirror)
{
	mirror = isBoxMirror;
}
