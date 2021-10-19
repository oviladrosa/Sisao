#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h" 
#include "Game.h"
#include "StateManager.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 4
#define INIT_MIRROR_PLAYER_X_TILES 6
#define INIT_MIRROR_PLAYER_Y_TILES 15


Scene::Scene(CStateManager* pManager) 
	: CGameState(pManager)
{
	map = NULL;
	player = NULL;
	mirrorPlayer = NULL;
	background = NULL;
	init();
}

Scene::~Scene()
{
	if (background != NULL)
		delete background;
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (mirrorPlayer != NULL)
		delete mirrorPlayer;
}

Scene* Scene::GetInstance(CStateManager* pManager)
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
	// initialization mirror Player
	mirrorPlayer = new Player();
	mirrorPlayer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	mirrorPlayer->setPosition(glm::vec2(INIT_MIRROR_PLAYER_X_TILES * map->getTileSize(), INIT_MIRROR_PLAYER_Y_TILES * map->getTileSize()));
	mirrorPlayer->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1)/2.f, float(SCREEN_HEIGHT - 1)/2.f, 0.f);
	currentTime = 0.0f;
}

void Scene::Update(DWORD deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	mirrorPlayer->update(deltaTime);
	if (Game::instance().getKey(27)) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}
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
	radiopool->renderTransparent();
	
}

void Scene::Reset() {
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



