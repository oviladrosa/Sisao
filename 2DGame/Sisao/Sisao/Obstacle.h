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
	
	virtual bool LeftCollision(glm::vec2& posCollider) { return false; }
	virtual bool RightCollision(glm::vec2& posCollider) { return false; }
	virtual bool UpperCollision(glm::vec2& posCollider) { return false; }
	virtual bool BottomCollision(glm::vec2& posCollider) { return false; }



protected:
	glm::vec2 getPosition();
	glm::ivec2 tileMapDispl;
	glm::vec2 initialPosition;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif	//_OBSTACLE_H
