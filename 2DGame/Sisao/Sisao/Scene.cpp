#include <iostream>
#include <cmath>
#include <stdlib.h>
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

ISoundEngine* SoundEngine2 = createIrrKlangDevice();

unsigned int nr_particles = 500;
unsigned int nr_new_particles = 2;



Scene::Scene(CSceneManager* pManager)
	: CGameScene(pManager)
{
	map = NULL;
	player = NULL;
	mirrorPlayer = NULL;
	background = NULL;
	card1 = NULL;
	card2 = NULL;
	hammerList = list<HydraulicPress*>();
	boxList = list<Box*>();
	wallList = list<Wall*>();
	wallListAux = list<Wall*>();
	transporterList = list<Transporter*>();
	spikeList = list<Spike*>();
	decorationList = list<Sprite*>();
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
	if (hammerList.empty()) {
		for (HydraulicPress* hammer : hammerList) delete hammer;
		hammerList.clear();
	}
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
	if (!transporterList.empty()) {
		for (Transporter* trans: transporterList) delete trans;
		transporterList.clear();
	}
	if (!spikeList.empty()) {
		for (Spike* sp : spikeList) delete sp;
		spikeList.clear();
	}
	if (!decorationList.empty()) {
		for (Sprite* sprite : decorationList) delete sprite;
		decorationList.clear();
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

	float half_point = float(playerPostion.x + mirrorplayerPostion.x) / 2.0f;
	projection = glm::ortho((half_point*32.f)-SCREEN_WIDTH/4.f, (half_point * 32.f) + SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1)/2.f, 0.f);
	currentTime = 0.0f;
	SoundEngine2->play2D("audio/gameloop.mp3", true);
	finished = false;
	startHammerSound = true;


	ShaderProgram particleSystem = initializeParticleShader();
	Texture particleTex;
	particleTex.loadFromFile("images/dirt_01.png", TEXTURE_PIXEL_FORMAT_RGBA);
	Particles = new ParticleGenerator(
		particleSystem,
		particleTex,
		500
	);

	Particles2 = new ParticleGenerator(
		particleSystem,
		particleTex,
		500
	);

	deathExplode = new ParticleGenerator(
		particleSystem,
		particleTex,
		200
	);
	GodMode = false;
	removeBarrier = false;
}

void Scene::Update(DWORD deltaTime)
{
	currentTime += deltaTime;
	checkTransporterCollisions();

	if (Game::instance().getKey(103)) {
		GodMode = !GodMode;
		Game::instance().keyReleased(103);
	}
	if (Game::instance().getKey(114) && !removeBarrier) {
		removeBarrier = true;
	}
	if (!finished) {
		player->update(deltaTime);
		mirrorPlayer->update(deltaTime);
	}
	card1->update(deltaTime);
	card2->update(deltaTime);
	glm::vec4 camActual;
	if (player->getPosition()[0] < SCREEN_WIDTH / 4.f || mirrorPlayer->getPosition()[0] < SCREEN_WIDTH / 4.f) {
		projection = glm::ortho(0.f, float(SCREEN_WIDTH / 2.f), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
		camActual = glm::vec4(0.f, float(SCREEN_WIDTH / 2.f), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else if (player->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f)) || mirrorPlayer->getPosition()[0] > (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f))) {
		projection = glm::ortho(float(SCREEN_WIDTH / 2.f), float(SCREEN_WIDTH), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
		camActual = glm::vec4(float(SCREEN_WIDTH / 2.f), float(SCREEN_WIDTH), float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	else {
		float half_point = (player->getPosition()[0] + mirrorPlayer->getPosition()[0]) / 2.f;
		projection = glm::ortho((half_point)-SCREEN_WIDTH / 4.f, (half_point)+SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
		camActual = glm::vec4((half_point)-SCREEN_WIDTH / 4.f, (half_point)+SCREEN_WIDTH / 4.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	}
	if (lever != NULL) {
		lever->update(deltaTime);
		if (lever->isPlayerTouching(glm::vec2(player->getPosition()))
			|| lever->isPlayerTouching(glm::vec2(mirrorPlayer->getPosition()))) 
		{
			SoundEngine2->play2D("audio/lever.wav", false);
			lever->setEnabled(true);
		}
		if (lever->isEnabled() || removeBarrier) wallList.clear();
		else
		{
			for (Wall* wall : wallList)
				wall->update(deltaTime);
		}
	}
	for (Box* box : boxList)
		box->update(deltaTime);

	bool detectedImpact = false;
	for (HydraulicPress* hammer : hammerList)
	{
		if (startHammerSound && hammer->getAnimation() == 1) {
			//SoundEngine2->play2D("audio/hammer-complete-9.wav", true);
			startHammerSound = false;
		}
		if (hammer->getAnimation() == 0 && !detectedImpact) {
			detectedImpact = true;
			for (float f = 0.0f; f <= 2.0f; f+=0.1f) {
				float left = float(rand() % 5 + camActual.x - 5.f);
				float right = float(rand() % 5 + camActual.y - 5.f);
				float bottom = float(rand() % 5 + camActual.z - 5.f);
				float top = float(rand() % 5 + camActual.w - 5.f);

				projection = glm::ortho(camActual.x, right, bottom, camActual.w);
			}
		}
		hammer->update(deltaTime);
	}
/*	if (Game::instance().getKey(27)) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}*/
	if (player->isDead() || mirrorPlayer->isDead()) { 
		SoundEngine2->play2D("audio/dead-sound.wav", false);
		Reset(); 
	}



	if (card1->isPlayerTouching(player->getPosition()) && card2->isPlayerTouching(mirrorPlayer->getPosition())) {
		if (!finished) {
			card1->changeAnimation(1);
			card2->changeAnimation(1);
			finished = true;
		}
	}

	Particles->Update(deltaTime, *player, 2, glm::vec2(2.0f));
	Particles2->Update(deltaTime, *mirrorPlayer, 2, glm::vec2(2.0f));
	deathExplode->easyUpdate(deltaTime);

	for (Transporter* t : transporterList)
		t->update(deltaTime);

	checkWallCollisions();
	checkBoxCollisions();
	if (!GodMode) {
		checkSpikeCollisions();
		checkHydraulicPressCollisions();
	}

	for (Sprite* s : decorationList) s->update(deltaTime);
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
	for (Sprite* sprite : decorationList) sprite->render();
	player->render();
	mirrorPlayer->render();
	card1->render();
	card2->render();
	for(Wall* wall : wallList) wall->render();
	for(Box* box : boxList) box->render();
	for (Transporter* t : transporterList) t->render();
	for (Spike* s : spikeList) s->render();
	for (HydraulicPress* hammer : hammerList) hammer->render();
	if (lever != NULL) lever->render();
	radiopool->renderTransparent();

	if(player->getVelocity().x != 0.f && player->getVelocity().y == 0.f) 
		Particles->Draw(projection);
	if (mirrorPlayer->getVelocity().x != 0.f && mirrorPlayer->getVelocity().y == 0.f)
		Particles2->Draw(projection);
	deathExplode->Draw(projection);
	
}

void Scene::Reset() {
	if (lever != NULL && (lever->isEnabled() || removeBarrier))
	{
		lever->setEnabled(false);
		for (Wall* wall : wallListAux)
		{
			wallList.push_back(wall);
			wall->render();
			wall->setIsActive(true);
		}
	}
	for (Box* box : boxList)
	{
		box->setPosition(glm::vec2(box->getInitialPosition().x * map->getTileSize(), box->getInitialPosition().y * map->getTileSize()));
	}
	
	player->setPosition(glm::vec2(playerPostion.x * map->getTileSize(), playerPostion.y * map->getTileSize()));
	mirrorPlayer->setPosition(glm::vec2(mirrorplayerPostion.x * map->getTileSize(), mirrorplayerPostion.y * map->getTileSize()));
	finished = false;
	removeBarrier = false;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::EnterScene() {
	GodMode = false;
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



void Scene::addTransporter(glm::vec2 pos, bool left) {
	Transporter* transporter = new Transporter();
	transporter->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	transporter->setTileMap(map);
	transporter->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	transporter->setDirection(left);
	transporterList.push_back(transporter);
}

void Scene::checkTransporterCollisions() {
	for (Transporter* trans : transporterList) {
		if (trans->LeftCollision(player->getPosition(), glm::ivec2(32, 32))) {
			player->setPosition(glm::vec2(player->getPosition().x + 2, player->getPosition().y));
		}
		if (trans->RightCollision(player->getPosition(), glm::ivec2(32, 32))) {
			player->setPosition(glm::vec2(player->getPosition().x - 2, player->getPosition().y));
		}
		if (trans->UpperCollision(player->getPosition(), glm::ivec2(32, 32))) {
			player->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y + 2));
		}
		if (trans->BottomCollision(player->getPosition(), glm::ivec2(32, 32))) {
			player->setPosition(glm::vec2(player->getPosition().x + trans->getMovement(), player->getPosition().y - 4));
			player->setTransporterCollision(true);
		}

		if (trans->LeftCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x + 2, mirrorPlayer->getPosition().y));
		}
		if (trans->RightCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x - 2, mirrorPlayer->getPosition().y));
		}
		if (trans->UpperCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x + trans->getMovement(), mirrorPlayer->getPosition().y + 4));
			mirrorPlayer->setTransporterCollision(true);
		}
		
		
	}
}

void Scene::addSpike(glm::vec2 pos, bool mirror) {
	Spike* spike= new Spike();
	spike->setSpike(mirror);
	spike->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	spike->setTileMap(map);
	spike->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	spikeList.push_back(spike);
}

void Scene::checkSpikeCollisions() {
	for (Spike* s : spikeList) {
		if (s->LeftCollision(player->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition())+16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->RightCollision(player->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->UpperCollision(player->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->BottomCollision(player->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}

		if (s->LeftCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->RightCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->UpperCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (s->BottomCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32))) {
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()) + 16.f, glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
	}
}

ShaderProgram Scene::initializeParticleShader() {
	ShaderProgram aux;
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/particle.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/particle.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	aux.init();
	aux.addShader(vShader);
	aux.addShader(fShader);
	aux.link();
	if (!aux.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	aux.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
	return aux;
}

void Scene::addBox(glm::vec2 pos, bool mirror)
{
	Box* box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	box->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	box->setInitialPosition(pos);
	box->setTileMap(map);
	box->setMirror(mirror);
	boxList.push_back(box);
}

void Scene::checkBoxCollisions()
{
	for (Box* box : boxList)
	{
		int posY;

		if (box->LeftCollision(player->getPosition(), glm::ivec2(32, 32)))
		{
			if (!box->isCollisioning())
			{
				player->setPosition(glm::vec2(player->getPosition().x + 1, player->getPosition().y));
				box->setPosition(glm::vec2(box->getPosition().x - 3, box->getPosition().y));
				box->setLastDirection(false);
			}
			else
			{
				player->setPosition(glm::vec2(player->getPosition().x + 3, player->getPosition().y));
			}
			//if (box->isFalling()) box->setPosition(glm::vec2(box->getPosition().x - 1, box->getPosition().y));
		}
		if (box->LeftCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)))
		{
			if (!box->isCollisioning())
			{
				mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x + 1, mirrorPlayer->getPosition().y));
				box->setPosition(glm::vec2(box->getPosition().x - 3, box->getPosition().y));
				box->setLastDirection(false);
			}
			else
			{
				mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x + 3, mirrorPlayer->getPosition().y));
			}
			//if (box->isFalling()) box->setPosition(glm::vec2(box->getPosition().x - 1, box->getPosition().y));
		}
		if (box->RightCollision(player->getPosition(), glm::ivec2(32, 32)))
		{
			if (!box->isCollisioning())
			{
				player->setPosition(glm::vec2(player->getPosition().x - 1, player->getPosition().y));
				box->setPosition(glm::vec2(box->getPosition().x + 3, box->getPosition().y));
				box->setLastDirection(true);
			}
			else
			{
				player->setPosition(glm::vec2(player->getPosition().x - 3, player->getPosition().y));
			}
			//if (box->isFalling()) box->setPosition(glm::vec2(box->getPosition().x + 1, box->getPosition().y));
		}
		if (box->RightCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)))
		{
			if (!box->isCollisioning())
			{
				mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x - 1, mirrorPlayer->getPosition().y));
				box->setPosition(glm::vec2(box->getPosition().x + 3, box->getPosition().y));
				box->setLastDirection(true);
			}
			else
			{
				mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x - 3, mirrorPlayer->getPosition().y));
			}
			//if (box->isFalling()) box->setPosition(glm::vec2(box->getPosition().x + 1, box->getPosition().y));
		}

		if (box->BottomCollision(player->getPosition(), glm::ivec2(32, 32), &posY))
		{
			player->setPosition(glm::vec2(player->getPosition().x, posY));
			box->setPosition(glm::vec2(box->getPosition().x, box->getPosition().y));
			player->setTransporterCollision(true);
		}
		if (box->UpperCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32), &posY))
		{
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x, posY));
			box->setPosition(glm::vec2(box->getPosition().x, box->getPosition().y));
			mirrorPlayer->setTransporterCollision(true);
		}
		
	}
}

void Scene::addWall(glm::vec2 pos)
{
	Wall* wall = new Wall();
	wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	wall->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	wall->setTileMap(map);
	wallList.push_back(wall);
	wallListAux.push_back(wall);
}

void Scene::checkWallCollisions()
{
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
			mirrorPlayer->setPosition(glm::vec2(mirrorPlayer->getPosition().x - 2, mirrorPlayer->getPosition().y));
			mirrorPlayer->forceAnimation(1); //STAND_RIGHT
		}
	}
}

void Scene::addHammer(glm::vec2 pos, bool mirror) {
	HydraulicPress* hammer = new HydraulicPress();
	hammer->setMirror(mirror);
	hammer->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	hammer->setTileMap(map);
	hammer->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
	hammerList.push_back(hammer);
}

void Scene::checkHydraulicPressCollisions()
{
	int intY;
	for (HydraulicPress* hammer : hammerList)
	{

		if (hammer->LeftCollision(player->getPosition(), glm::ivec2(32, 32)) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		else if (hammer->LeftCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (hammer->RightCollision(player->getPosition(), glm::ivec2(32, 32)) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		else if (hammer->RightCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32)) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (hammer->UpperCollision(player->getPosition(), glm::ivec2(32, 32), &intY) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(player->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
		if (hammer->BottomCollision(mirrorPlayer->getPosition(), glm::ivec2(32, 32), &intY) && hammer->getAnimation() == 0)
		{
			deathExplode->respawnAllParticles(glm::vec2(mirrorPlayer->getPosition()), glm::vec2(2.0f));
			SoundEngine2->play2D("audio/hit.wav", false);
			Reset();
		}
	}
}

void Scene::addLever(glm::vec2 pos, bool mirror)
{
	lever = new Lever();
	lever->setMirror(mirror);
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	lever->setTileMap(map);
	lever->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
}

void Scene::addPointer(glm::vec2 position, bool mirror) {
	if (!mirror) {
		pointer.loadFromFile("images/Pointer1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		pointer.setMinFilter(GL_NEAREST);
		pointer.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f),
			&pointer, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else { 
		pointerMirror.loadFromFile("images/Pointer1-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA); 
		pointerMirror.setMinFilter(GL_NEAREST);
		pointerMirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f),
			&pointerMirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addNum1(glm::vec2 position, bool mirror) {
	if (!mirror) {
		num1.loadFromFile("images/1.png", TEXTURE_PIXEL_FORMAT_RGBA); 
		num1.setMinFilter(GL_NEAREST);
		num1.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num1, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		num1Mirror.loadFromFile("images/1-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA); 
		num1Mirror.setMinFilter(GL_NEAREST);
		num1Mirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num1Mirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addNum2(glm::vec2 position, bool mirror) {
	if (!mirror) {
		num2.loadFromFile("images/2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num2.setMinFilter(GL_NEAREST);
		num2.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num2, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		num2Mirror.loadFromFile("images/2-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num2Mirror.setMinFilter(GL_NEAREST);
		num2Mirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num2Mirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addNum3(glm::vec2 position, bool mirror) {
	if (!mirror) {
		num3.loadFromFile("images/3.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num3.setMinFilter(GL_NEAREST);
		num3.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num3, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		num3Mirror.loadFromFile("images/3-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num3Mirror.setMinFilter(GL_NEAREST);
		num3Mirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num3Mirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addNum4(glm::vec2 position, bool mirror) {
	if (!mirror) {
		num4.loadFromFile("images/4.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num4.setMinFilter(GL_NEAREST);
		num4.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num4, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		num4Mirror.loadFromFile("images/4-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num4Mirror.setMinFilter(GL_NEAREST);
		num4Mirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num4Mirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addNum5(glm::vec2 position, bool mirror) {
	if (!mirror) {
		num5.loadFromFile("images/5.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num5.setMinFilter(GL_NEAREST);
		num5.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num5, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		num5Mirror.loadFromFile("images/5-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA);
		num5Mirror.setMinFilter(GL_NEAREST);
		num5Mirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&num5Mirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addFlag(glm::vec2 position, bool mirror) {
	if (!mirror) {
		flag.loadFromFile("images/Flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
		flag.setMinFilter(GL_NEAREST);
		flag.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 64.f), glm::vec2(1.f, 1.f),
			&flag, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else {
		flagMirror.loadFromFile("images/Flag-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA);
		flagMirror.setMinFilter(GL_NEAREST);
		flagMirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 64.f), glm::vec2(1.f, 1.f),
			&flagMirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
}

void Scene::addMop(glm::vec2 position, bool mirror) {
	if (!mirror) { 
		mop.loadFromFile("images/Mop.png", TEXTURE_PIXEL_FORMAT_RGBA); 
		mop.setMinFilter(GL_NEAREST);
		mop.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&mop, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}
	else { 
		mopMirror.loadFromFile("images/Mop-mirror.png", TEXTURE_PIXEL_FORMAT_RGBA); 
		mopMirror.setMinFilter(GL_NEAREST);
		mopMirror.setMagFilter(GL_NEAREST);
		Sprite* sprite = Sprite::createSprite(glm::vec2(32.f, 36.f), glm::vec2(1.f, 1.f),
			&mopMirror, &texProgram);
		sprite->setPosition(glm::vec2(position.x * map->getTileSize(), position.y * map->getTileSize()));
		decorationList.push_back(sprite);
	}


}




