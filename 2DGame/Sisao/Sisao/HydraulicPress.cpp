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
	if(!mirror)
		spritesheet.loadFromFile("images/Hammer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/HammerInverted.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(PRESS_DOWN, 8);
	//sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 0.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 1.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 2.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 3.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 4.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 5.f, 0.f));
	sprite->addKeyframe(PRESS_DOWN, glm::vec2(0.125f * 6.f, 0.f));
	
	sprite->setAnimationSpeed(PRESS_UP, 2);
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 7.f, 0.f));
	sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 0.f, 0.f));
	//sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 1.f, 0.f));
	//sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 2.f, 0.f));
	//sprite->addKeyframe(PRESS_UP, glm::vec2(0.125f * 3.f, 0.f));

	sprite->changeAnimation(0);
	lastIDLEAnimationChange = 0;
	lastHitAnimationChange = 0;
	tileMapDispl = tileMapPos;
	initialPosition = glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));

}

void HydraulicPress::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (sprite->animation() == 1) {
		lastHitAnimationChange = 0;
		lastIDLEAnimationChange++;
	}
	else {
		lastIDLEAnimationChange = 0;
		lastHitAnimationChange++;
		if(lastHitAnimationChange == 33) sprite->changeAnimation(1);
	}
	if (lastIDLEAnimationChange == 100) sprite->changeAnimation(0);
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
	posObstacle = pos;
	initialPosition = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + initialPosition.x), float(tileMapDispl.y + initialPosition.y)));
}

bool HydraulicPress::isPlayerTouching(glm::vec2& posPlayer)
{
	return (float(tileMapDispl.x + posPlayer.x) == float(tileMapDispl.x + posObstacle.x))
		&& (float(tileMapDispl.y + posPlayer.y) == float(tileMapDispl.y + posObstacle.y));
}

void HydraulicPress::setMirror(bool mir)
{
	mirror = mir;
}

bool HydraulicPress::LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size)
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

bool HydraulicPress::RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size)
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

bool HydraulicPress::UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY)
{
	float x0, x1, y;

	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y);
	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x) && y == float(posObstacle.y + size.y))
		{
			return true;
		}
	}
	return false;
}

bool HydraulicPress::BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY)
{
	float x0, x1, y;

	x0 = float(posCollider.x + 10);
	x1 = float(posCollider.x + size.x - 10);
	y = float(posCollider.y + size.y);
	for (float x = x0; x <= x1; x += 0.1f)
	{
		if (x >= float(posObstacle.x) && x <= float(posObstacle.x + size.x) && y == float(posObstacle.y + size.y))
		{
			return true;
		}
	}
	return false;
}

int HydraulicPress::getAnimation() const 
{
	return sprite->animation();
}