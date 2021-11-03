#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <list>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Card.h"
#include "Spike.h"

#include "GameScene.h"
#include "SceneManager.h"
#include "Obstacle.h"
#include "HydraulicPress.h"
#include "Box.h"
#include "Wall.h"
#include "Lever.h"
#include "Transporter.h"
#include "GameState.h"
#include "ParticleGenerator.h"

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
	void addLever(glm::vec2 pos, bool mirror);
	void addWall(glm::vec2 pos);
	void checkWallCollisions();
	void addBox(glm::vec2 pos, bool mirror);
	void checkBoxCollisions();
	void addHammer(glm::vec2 pos, bool mirror);
	void checkHydraulicPressCollisions();
	void setNextScene(int x);
	bool isFinished();
	int getNextScene();
	void addTransporter(glm::vec2 pos, bool left);
	void checkTransporterCollisions();
	void addSpike(glm::vec2 pos, bool mirror);
	void checkSpikeCollisions();
	void addPointer(glm::vec2 position, bool mirror);
	void addNum1(glm::vec2 position, bool mirror);
	void addNum2(glm::vec2 position, bool mirror);
	void addNum3(glm::vec2 position, bool mirror);
	void addNum4(glm::vec2 position, bool mirror);
	void addNum5(glm::vec2 position, bool mirror);
	void addFlag(glm::vec2 position, bool mirror);
	void addMop(glm::vec2 position, bool mirror);
	ShaderProgram initializeParticleShader();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Player* mirrorPlayer;
	Card* card1;
	Card* card2;
	list<HydraulicPress*> hammerList;
	list<Box*> boxList;
	list<Wall*> wallList;
	list<Wall*> wallListAux;
	list<Transporter*> transporterList;
	list<Spike*> spikeList;
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
	ParticleGenerator* Particles;
	ParticleGenerator* Particles2;
	ParticleGenerator* deathExplode;
	bool GodMode;
	bool removeBarrier;
	list<Sprite*> decorationList;
	//Textures for decoration
	Texture pointer;
	Texture num1;
	Texture num2;
	Texture num3;
	Texture num4;
	Texture num5;
	Texture flag;
	Texture mop;
	Texture pointerMirror;
	Texture num1Mirror;
	Texture num2Mirror;
	Texture num3Mirror;
	Texture num4Mirror;
	Texture num5Mirror;
	Texture flagMirror;
	Texture mopMirror;
	bool startHammerSound;
};


#endif // _SCENE_INCLUDE

