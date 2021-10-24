#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Card.h"

#include "SceneState.h"
#include "SceneManager.h"
#include "GameState.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene : public CSceneState
{

public:
	static Scene* GetInstance(CSceneManager* sManager);
	~Scene();

	void init();
	void Update(DWORD deltaTime);
	void Draw();
	void Reset();

protected:
	Scene(CSceneManager* sManager);
	

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Player* mirrorPlayer;
	Card* card1;
	Card* card2;
	Texture bgText;
	ShaderProgram texProgram;
	Sprite* background;
	float currentTime;
	glm::mat4 projection;
	Texture rpText;
	Sprite* radiopool;

	CSceneManager* m_pSceneManager;
};


#endif // _SCENE_INCLUDE

