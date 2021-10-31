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
#include "Transporter.h"
#include "GameState.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene : public CGameScene
{

public:
	Scene(CSceneManager* pManager);
	~Scene();

	void init();
	void Update(DWORD deltaTime);
	void Draw();
	void Reset();
	void EnterScene();
	void LeaveScene();
	void setLevel(string s);
	void setcard1Postion(glm::vec2 pos);
	void setcard2Postion(glm::vec2 pos);
	void setplayerPostion(glm::ivec2 pos);
	void setmirrorplayerPostion(glm::ivec2 pos);
	void setNextScene(int x);
	bool isFinished();
	int getNextScene();
	void addTransporter(glm::vec2 pos, bool left);
	void checkTransporterCollisions();

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
	list<Transporter*> transporterList;
	Lever* lever;
	Texture bgText;
	ShaderProgram texProgram;
	Sprite* background;
	float currentTime;
	glm::mat4 projection;
	Texture rpText;
	Sprite* radiopool;
	string s_level;
	bool finished;
	glm::vec2 card1Position;
	glm::vec2 card2Position;
	glm::ivec2 playerPostion;
	glm::ivec2 mirrorplayerPostion;
	int nextScene;
};


#endif // _SCENE_INCLUDE

