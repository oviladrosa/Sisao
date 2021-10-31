#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "Sprite.h"
#include "TileMap.h"

class Obstacle
{
public:

	Obstacle();
	virtual ~Obstacle();

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) { }
	virtual void update(int deltaTime) {}
	virtual void render() {}

	virtual void setTileMap(TileMap* tileMap) {}
	virtual void setPosition(const glm::vec2& pos) {}
	virtual bool isPlayerTouching(glm::vec2& posPlayer) { return false;}
	
	virtual bool LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) { return false; }
	virtual bool RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) { return false; }
	virtual bool UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) { return false; }
	virtual bool BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size, int* posY) { return false; }

	virtual glm::ivec2 getPosition();

protected:
	
	glm::ivec2 tileMapDispl, posObstacle;
	glm::vec2 initialPosition;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif	//_OBSTACLE_H
