#ifndef _BOX_H
#define _BOX_H

#include "Obstacle.h"

class Box : public Obstacle
{

public:
	Box();
	~Box();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isPlayerTouching(glm::vec2& posPlayer);

	bool LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	bool RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	bool UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) const;
	bool BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) const;

};

#endif	//_BOX_H