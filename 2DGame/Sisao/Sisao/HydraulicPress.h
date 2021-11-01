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
	
	bool LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size);
	bool RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size);
	bool UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY);
	bool BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY);
};

#endif	//_HYDRAULICPRESS_H