#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h" 
#include "Game.h"
#include "StateManager.h"
#include <irrKlang.h>
using namespace irrklang;


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 8
#define INIT_PLAYER_Y_TILES 5
#define INIT_MIRROR_PLAYER_X_TILES 8
#define INIT_MIRROR_PLAYER_Y_TILES 15
#define INIT_CARD1_X_TILES 6
#define INIT_CARD1_Y_TILES 2
#define INIT_CARD2_X_TILES 6
#define INIT_CARD2_Y_TILES 15
#define INIT_HAMMER_X_TILES 18
#define INIT_HAMMER_Y_TILES 8
#define INIT_WALL1_X_TILES 28
#define INIT_WALL1_Y_TILES 6
#define INIT_WALL2_X_TILES 25
#define INIT_WALL2_Y_TILES 7
#define INIT_BOX_X_TILES 34
#define INIT_BOX_Y_TILES 7
#define INIT_LEVER_X_TILES 36 
#define INIT_LEVER_Y_TILES 7

ISoundEngine* SoundEngine2 = createIrrKlangDevice();

Scene::Scene(CSceneManager* pManager)
	: CGameScene(pManager)
{
	map = NULL;
	player = NULL;
	mirrorPlayer = NULL;
	background = NULL;
	card1 = NULL;
	card2 = NULL;
	hammer = NULL;
	boxList = list<Box*>();
	wallList = list<Wall*>();
	wallListAux = list<Wall*>();
	lever = NULL;
}

Scene::~Scene()
{
	if (background != NULL)
		delete background;
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (mirrorPlayer != NULL)
		delete mirrorPlayer;
	if (card1 != NULL)
		delete card1;
	if (card2 != NULL)
		delete card2;
	if (hammer != NULL)
		delete hammer;
	if (!wallList.empty()) {
		for (Wall* wall : wallList) delete wall;
		wallList.clear();
	}
	if (!wallListAux.empty()) {
		for (Wall* wall : wallListAux) delete wall;
		wallListAux.clear();
	}
	if (!boxList.empty()) {
		for (Box* box : boxList) delete box;
		boxList.clear();
	}
	if (lever != NULL)
		delete lever;
}


void Scene::init()
{
	initShaders();
	//Background initialization
	bgText.loadFromFile("images/Background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText.setMinFilter(GL_NEAREST);
	bgText.setMagFilter(GL_NEAREST);
	float xScale = 1.f - (bgText.width() / 1920.f);
	float yScale = 1.f - (bgText.height() / 1080.f);
	background = Sprite::createSprite(glm::vec2(1920.f, 1080.f), glm::vec2(1.f + xScale, 1.f + yScale),
		&bgText, &texProgram);
	background->setPosition(background->position());

	//Radioactive pool initialization
	rpText.loadFromFile("images/radioactive_pool.png", TEXTURE_PIXEL_FORMAT_RGBA);
	rpText.setMinFilter(GL_NEAREST);
	rpText.setMagFilter(GL_NEAREST);
	radiopool = Sprite::createSprite(glm::vec2(2048.f, 512.f), glm::vec2(1.f, 1.f), &rpText, &texProgram);
	radiopool->setPosition(glm::vec2(0.f, 170.f));

	map = TileMap::createTileMap(s_level, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	// initialization upper Player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	player->setPosition(glm::vec2(playerPostion.x * map->getTileSize(), playerPostion.y * map->getTileSize()));
	player->setTileMap(map);
	player->setSoundEngine(SoundEngine2);
	// initialization mirror Player
	mirrorPlayer = new Player();
	mirrorPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	mirrorPlayer->setPosition(glm::vec2(mirrorplayerPostion.x * map->getTileSize(), mirrorplayerPostion.y * map->getTileSize()));
	mirrorPlayer->setTileMap(map);
	mirrorPlayer->setSoundEngine(SoundEngine2);
	//initialization card 1
	card1 = new Card();
	card1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	card1->setPosition(glm::vec2(glm::vec2(card1Position.x * map->getTileSize(), card1Position.y * map->getTileSize())));
	card1->setTileMap(map);
	//initialization card 2
	card2 = new Card();
	card2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	card2->setPosition(glm::vec2(glm::vec2(card2Position.x * map->getTileSize(), card2Position.y * map->getTileSize())));
	card2->setTileMap(map);

	hammer = new HydraulicPress();
	hammer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	hammer->setPosition(glm::vec2(INIT_HAMMER_X_TILES * map->getTileSize(), INIT_HAMMER_Y_TILES * map->getTileSize()));
	hammer->setTileMap(map);

	Box* box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	box->setPosition(glm::vec2(INIT_BOX_X_TILES * map->getTileSize(), INIT_BOX_Y_TILES * map->getTileSize()));
	box->setTileMap(map);
	boxList.push_back(box);

	Wall* wall = new Wall();
	wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	wall->setPosition(glm::vec2(INIT_WALL1_X_TILES * map->getTileSize(), INIT_WALL1_Y_TILES * map->getTileSize()));
	wall->setTileMap(map);
	wallList.push_back(wall);
	wallListAux.push_back(wall);

	Wall* wall2 = new Wall();
	wall2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	wall2->setPosition(glm::vec2(INIT_WALL2_X_TILES * map->getTileSize(), INIT_WALL2_Y_TILES * map->getTileSize()));
	wall2->setTileMap(map);
	wallList.push_back(wall2);
	wallListAux.push_back(wall2);

	lever = new Lever();
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	lever->setPosition(glm::vec2(INIT_LEVER_X_TILES * map->getTileSize(), INIT_LEVER_Y_TILES * map->getTileSize()));
	lever->setTileMap(map);


	float half_point = (INIT_PLAYER_X_TILES + INIT_MIRROR_PLAYER_X_TILES) / 2.0;
	projection = glm::ortho((half_point*32.f)-SCREEN_WIDTH/4.f, (half_point * 32.f) + SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1)/2.f, 0.f);
	currentTime = 0.0f;
	SoundEngine2->play2D("audio/gameloop.mp3", true);
	finished = false;
}

void Scene::Update(DWORD deltaTime)
{
	currentTime += deltaTime;
	if (!finished) {
		player->update(deltaTime);
		mirrorPlayer->update(deltaTime);
	}
	card1->update(deltaTime);
	card2->update(deltaTime);
	if (player->getPosition()[0] < SCREEN_WIDTH / 4.f || mirrorPlayer->getPosition()[0] < SCREEN_WIDTH / 4.f) {
		projection = glm::ortho(0.f, float(SCREEN_WIDTH / 2.f), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else if (player->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f)) || mirrorPlayer->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f))) {
		projection = glm::ortho(float(SCREEN_WIDTH / 2.f), float(SCREEN_WIDTH), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else {
		float half_point = (player->getPosition()[0] + mirrorPlayer->getPosition()[0]) / 2.f;
		projection = glm::ortho((half_point)-SCREEN_WIDTH / 4.f, (half_point)+SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	hammer->update(deltaTime);
	lever->update(deltaTime);
	if (lever->isPlayerTouching(glm::vec2(player->getPosition()))
		|| lever->isPlayerTouching(glm::vec2(mirrorPlayer->getPosition()))) lever->setEnabled(true);
	if (lever->isEnabled()) wallList.clear();
	else
	{
		for (Wall* wall : wallList)
			wall->update(deltaTime);
	}
	for (Box* box : boxList)
		box->update(deltaTime);

	if (player->getPosition()[0] < SCREEN_WIDTH / 4.f || mirrorPlayer->getPosition()[0] < SCREEN_WIDTH / 4.f) {
		projection = glm::ortho(0.f, float(SCREEN_WIDTH / 2.f), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else if (player->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f)) || mirrorPlayer->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f))) {
		projection = glm::ortho(float(SCREEN_WIDTH / 2.f), float(SCREEN_WIDTH), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else {
		float half_point = (player->getPosition()[0] + mirrorPlayer->getPosition()[0]) / 2.f;
		projection = glm::ortho((half_point)-SCREEN_WIDTH / 4.f, (half_point)+SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
/*	if (Game::instance().getKey(27)) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}*/
	if (player->isDead() || mirrorPlayer->isDead()) Reset();

	if (card1->isPlayerTouching(player->getPosition()) && card2->isPlayerTouching(mirrorPlayer->getPosition())) {
		if (!finished) {
			card1->changeAnimation(1);
			card2->changeAnimation(1);
			finished = true;
		}
	}

	for (Wall* wall : wallList)
	{
		
		if (wall->LeftCollision(player->getPosition(), glm::ivec2(32, 32)))
		{
			player->setPosition(glm::vec2(player->getPosition().x + 2, player->getPosition().y));
			player->forceAnimation(0); //STAND_LEFT
		}
		else if (wall->LeftCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)))
		{
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x + 2, mirrorPlayer->getPosition().y));
			mirrorPlayer->forceAnimation(0); //STAND_LEFT
		}
		if (wall->RightCollision(player->getPosition(), glm::ivec2(32, 32)))
		{
			player->setPosition(glm::vec2(player->getPosition().x - 2, player->getPosition().y));
			player->forceAnimation(1); //STAND_RIGHT
		}
		else if (wall->RightCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)))
		{
			mirrorPlayer->setPosition(glm::vec2(player->getPosition().x - 2, player->getPosition().y));
			mirrorPlayer->forceAnimation(1); //STAND_RIGHT
		}
	}
}

void Scene::Draw()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->renderTransparent();
	
	map->render();
	
	player->render();
	mirrorPlayer->render();
	card1->render();
	card2->render();
	hammer->render();
	for(Wall* wall : wallList) wall->render();
	for(Box* box : boxList) box->render();
	lever->render();
	radiopool->renderTransparent();
}

void Scene::Reset() {
	SoundEngine2->play2D("audio/dead-sound.wav", false);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	mirrorPlayer->setPosition(glm::vec2(INIT_MIRROR_PLAYER_X_TILES * map->getTileSize(), INIT_MIRROR_PLAYER_Y_TILES * map->getTileSize()));
	if (lever->isEnabled())
	{
		lever->setEnabled(false);
		for (Wall* wall : wallListAux)
		{
			wallList.push_back(wall);
			wall->render();
			wall->setIsActive(true);
		}
	}
	//cuidado aqui, tenemos que sustituir cada posicion inicial en su respectivo box.
	for (Box* box : boxList)
		box->setPosition(glm::vec2(INIT_BOX_X_TILES * map->getTileSize(), INIT_BOX_Y_TILES * map->getTileSize()));
	player->setPosition(glm::vec2(playerPostion.x * map->getTileSize(), playerPostion.y * map->getTileSize()));
	mirrorPlayer->setPosition(glm::vec2(mirrorplayerPostion.x * map->getTileSize(), mirrorplayerPostion.y * map->getTileSize()));
	finished = false;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::EnterScene(){
	SoundEngine2->play2D("audio/gameloop.mp3", true);
}

void Scene::LeaveScene() {
	SoundEngine2->stopAllSounds();
}

void Scene::setLevel(string s) {
	s_level = s;
}

void Scene::setcard1Postion(glm::vec2 pos) {
	card1Position = pos;
}

void Scene::setcard2Postion(glm::vec2 pos) {
	card2Position = pos;
}

void Scene::setplayerPostion(glm::ivec2 pos) {
	playerPostion = pos;
}

void Scene::setmirrorplayerPostion(glm::ivec2 pos) {
	mirrorplayerPostion = pos;
}

void Scene::setNextScene(int x) {
	nextScene = x;
}

int Scene::getNextScene() {
	return nextScene;
}

bool Scene::isFinished() {
	return finished;
}


