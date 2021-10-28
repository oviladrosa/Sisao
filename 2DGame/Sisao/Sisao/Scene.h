#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <list>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Card.h"

#include "GameScene.h"
#include "SceneManager.h"
#include "Obstacle.h"
#include "HydraulicPress.h"
#include "Box.h"
#include "Wall.h"
#include "Lever.h"
#include "GameState.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene : public CGameScene
{

public:
	static Scene* GetInstance(CSceneManager* pManager);
	~Scene();

	void init();
	void Update(DWORD deltaTime);
	void Draw();
	void Reset();
	void EnterScene();
	void LeaveScene();

protected:
	Scene(CSceneManager* pManager);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Player* mirrorPlayer;
	Card* card1;
	Card* card2;
	HydraulicPress* hammer;
	list<Box*> boxList;
	list<Wall*> wallList;
	list<Wall*> wallListAux;
	Lever* lever;
	Texture bgText;
	ShaderProgram texProgram;
	Sprite* background;
	float currentTime;
	glm::mat4 projection;
	Texture rpText;
	Sprite* radiopool;
};


#endif // _SCENE_INCLUDE

