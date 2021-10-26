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
	
protected:
	glm::vec2 getPosition();
	glm::ivec2 tileMapDispl;
	glm::vec2 initialPosition;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif	//_OBSTACLE_H
