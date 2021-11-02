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
	void setInitialPosition(const glm::vec2& pos);
	bool isPlayerTouching(glm::vec2& posPlayer);
	bool isMirror();
	bool isFalling() { return falling; }
	bool isCollisioning() { return collisioning; }
	void setMirror(bool isBoxMirror);

	bool LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size);
	bool RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size);
	bool UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY);
	bool BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY);
	void setLastDirection(bool ld);

private:
	bool mirror;
	bool falling;
	bool collisioning;
	int collisionAngle, startY;
	bool lastDirection;

};

#endif	//_BOX_H