#ifndef _LEVER_H
#define _LEVER_H

#include "Obstacle.h"

class Lever : public Obstacle
{

public:
	Lever();
	~Lever();

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

	bool isEnabled();
	void setEnabled(bool enable);

private:
	bool enabled;
};

#endif	//_LEVER_H