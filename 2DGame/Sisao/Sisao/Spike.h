#ifndef _SPIKE_H
#define _SPIKE_H

#include "Obstacle.h"

class Spike : public Obstacle
{

public:
	Spike();
	~Spike();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isPlayerTouching(glm::vec2& posPlayer);

	bool LeftCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	bool RightCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	bool UpperCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	bool BottomCollision(const glm::ivec2& posCollider, const glm::ivec2& size) const;
	void setSpike(bool mirr);

	bool mirror = false;
	
};

#endif	//_SPIKE_H
#pragma once
