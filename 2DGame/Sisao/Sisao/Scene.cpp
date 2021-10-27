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

#define INIT_PLAYER_X_TILES 30
#define INIT_PLAYER_Y_TILES 4
#define INIT_MIRROR_PLAYER_X_TILES 30
#define INIT_MIRROR_PLAYER_Y_TILES 15
#define INIT_CARD1_X_TILES 6
#define INIT_CARD1_Y_TILES 2
#define INIT_CARD2_X_TILES 6
#define INIT_CARD2_Y_TILES 15
#define INIT_HAMMER_X_TILES 18
#define INIT_HAMMER_Y_TILES 8
#define INIT_BOX_X_TILES 32 
#define INIT_BOX_Y_TILES 15
#define INIT_LEVER_X_TILES 36 
#define INIT_LEVER_Y_TILES 15

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
	box = NULL;
	lever = NULL;
	init();
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
	if (box != NULL)
		delete box;
	if (lever != NULL)
		delete lever;
}

Scene* Scene::GetInstance(CSceneManager* pManager)
{
	static Scene Instance(pManager);
	return &Instance;
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
	radiopool->setPosition(glm::vec2(0.f, 200.f));

	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	// initialization upper Player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setSoundEngine(SoundEngine2);
	// initialization mirror Player
	mirrorPlayer = new Player();
	mirrorPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	mirrorPlayer->setPosition(glm::vec2(INIT_MIRROR_PLAYER_X_TILES * map->getTileSize(), INIT_MIRROR_PLAYER_Y_TILES * map->getTileSize()));
	mirrorPlayer->setTileMap(map);
	mirrorPlayer->setSoundEngine(SoundEngine2);
	//initialization card 1
	card1 = new Card();
	card1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	card1->setPosition(glm::vec2(INIT_CARD1_X_TILES * map->getTileSize(), INIT_CARD1_Y_TILES * map->getTileSize()));
	card1->setTileMap(map);
	//initialization card 2
	card2 = new Card();
	card2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	card2->setPosition(glm::vec2(INIT_CARD2_X_TILES * map->getTileSize(), INIT_CARD2_Y_TILES * map->getTileSize()));
	card2->setTileMap(map);

	hammer = new HydraulicPress();
	hammer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	hammer->setPosition(glm::vec2(INIT_HAMMER_X_TILES * map->getTileSize(), INIT_HAMMER_Y_TILES * map->getTileSize()));
	hammer->setTileMap(map);

	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	box->setPosition(glm::vec2(INIT_BOX_X_TILES * map->getTileSize(), INIT_BOX_Y_TILES * map->getTileSize()));
	box->setTileMap(map);

	lever = new Lever();
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	lever->setPosition(glm::vec2(INIT_LEVER_X_TILES * map->getTileSize(), INIT_LEVER_Y_TILES * map->getTileSize()));
	lever->setTileMap(map);


	float half_point = (INIT_PLAYER_X_TILES + INIT_MIRROR_PLAYER_X_TILES) / 2.0;
	projection = glm::ortho((half_point*32.f)-SCREEN_WIDTH/4.f, (half_point * 32.f) + SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1)/2.f, 0.f);
	currentTime = 0.0f;
	SoundEngine2->play2D("audio/gameloop.mp3", true);
}

void Scene::Update(DWORD deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	mirrorPlayer->update(deltaTime);
	card1->update(deltaTime);
	card2->update(deltaTime);
	hammer->update(deltaTime);
	box->update(deltaTime);
	lever->update(deltaTime);
	float half_point = (player->getPosition()[0] + mirrorPlayer->getPosition()[0])/2.f;
	projection = glm::ortho((half_point) - SCREEN_WIDTH / 4.f, (half_point) + SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
/*	if (Game::instance().getKey(27)) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}*/
	if (player->isDead() || mirrorPlayer->isDead()) Reset();
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
	box->render();
	lever->render();
	radiopool->renderTransparent();
}

void Scene::Reset() {
	SoundEngine2->play2D("audio/dead-sound.wav", false);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	mirrorPlayer->setPosition(glm::vec2(INIT_MIRROR_PLAYER_X_TILES * map->getTileSize(), INIT_MIRROR_PLAYER_Y_TILES * map->getTileSize()));
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


