#ifndef _CARD_INCLUDE
#define _CARD_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Card is basically a Sprite that represents the flag. As such it has
// all properties it needs to track its positions.


class Card
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isPlayerTouching(glm::ivec2& posPlayer);
	void changeAnimation(int i);

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 initialPosition;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};


#endif // _CARD_INCLUDE

