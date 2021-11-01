#ifndef _TRANSPORTER_H
#define _TRANSPORTER_H

#include "Obstacle.h"

class Transporter : public Obstacle
{

public:
	Transporter();
	~Transporter();

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
	void setDirection(bool dir);
	int getMovement();



	bool left_direction;
};

#endif	//_TRANSPORTER_H
