#ifndef _WALL_H
#define _WALL_H

#include "Obstacle.h"

class Wall : public Obstacle
{

public:
	Wall();
	~Wall();

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

	bool isWallActive();
	void setIsActive(bool active);

private:
	bool isActive;
};

#endif	//_WALL_H