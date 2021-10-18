#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

#include "GameState.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene : public CGameState
{

public:
	static Scene* GetInstance(CStateManager* pManager);
	~Scene();

	void init();
	void Update(DWORD deltaTime);
	void Draw();

protected:
	Scene(CStateManager* pManager);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Player* mirrorPlayer;
	Texture bgText;
	ShaderProgram texProgram;
	Sprite* background;
	float currentTime;
	glm::mat4 projection;
	Texture rpText;
	Sprite* radiopool;
};


#endif // _SCENE_INCLUDE

