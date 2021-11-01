#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <irrKlang.h>
using namespace irrklang;


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool mirror);
	void update(int deltaTime);
	void render();
	bool isDead();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSoundEngine(ISoundEngine *SoundEngine);
	glm::ivec2 getPosition();
	void changeDemo(bool d);
	void forceAnimation(int a);
	bool isMirror();
	int getAnimation();
	void setTransporterCollision(bool tc);
	glm::vec2 getVelocity();
	bool isTouchingGround();
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::vec2 initialPosition;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool mirror;
	int lastMove;
	ISoundEngine* SoundEnginePlayer;
	bool demo;
	bool transporterColl;
	glm::vec2 velocityParticle;
};


#endif // _PLAYER_INCLUDE


