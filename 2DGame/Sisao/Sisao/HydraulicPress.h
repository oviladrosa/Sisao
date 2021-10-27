#ifndef _HYDRAULICPRESS_H
#define _HYDRAULICPRESS_H

#include "Obstacle.h"

class HydraulicPress : public Obstacle
{

public:
	HydraulicPress();
	~HydraulicPress();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isPlayerTouching(glm::vec2& posPlayer);
	
	bool LeftCollision(glm::vec2& posCollider);
	bool RightCollision(glm::vec2& posCollider);
	bool UpperCollision(glm::vec2& posCollider);
	bool BottomCollision(glm::vec2& posCollider);
};

#endif	//_HYDRAULICPRESS_H