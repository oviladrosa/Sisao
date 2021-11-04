#include "MenuState.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "StateManager.h"
#include "SceneState.h"
#include "InstructionsState.h"
#include "CreditsState.h"
#include <iostream>
#include "Scene.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

CMenuState::CMenuState(CStateManager* pManager)
	: CGameState(pManager), m_iCurrentSelection(0),
	m_pCurrentGame(NULL)
{
	initShaders();
	initTextShaders();
	opt = vector<TextBlock>();
	this->newBlock = new TextBlock(350.f, 150.f,60.f, 240.f, texProgram);
	opt.push_back(*newBlock);
	this->restartBlock = new TextBlock(350.f, 230.f, 60.f, 240.f, texProgram);
	opt.push_back(*restartBlock);

	this->optionsBlock = new TextBlock(350.f, 310.f, 60.f, 240.f, texProgram);
	opt.push_back(*optionsBlock);

	this->exitBlock = new TextBlock(350.f, 390.f, 60.f, 240.f, texProgram);
	opt.push_back(*exitBlock);

	if (!newText.init("fonts/Tourney-BoldItalic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!restartText.init("fonts/Tourney-BoldItalic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!optionsText.init("fonts/Tourney-BoldItalic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!exitText.init("fonts/Tourney-BoldItalic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!title.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 2.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	m_iCurrentSelection = 0;
	up = false;
	down = false;
	bgText_1.loadFromFile("images/bg_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText_1.setMinFilter(GL_NEAREST);
	bgText_1.setMagFilter(GL_NEAREST);
	
	background_1 = Sprite::createSprite(glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) / 2.f), glm::vec2(1.f, 1.f),
		&bgText_1, &texProgram2);
	background_1->setPosition(background_1->position());

	bgText_2.loadFromFile("images/bg_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText_2.setMinFilter(GL_NEAREST);
	bgText_2.setMagFilter(GL_NEAREST);

	background_2 = Sprite::createSprite(glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) / 2.f), glm::vec2(1.f, 1.f),
		&bgText_2, &texProgram2);
	background_2->setPosition(background_2->position());

	bgText_3.loadFromFile("images/bg_3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText_3.setMinFilter(GL_NEAREST);
	bgText_3.setMagFilter(GL_NEAREST);

	background_3 = Sprite::createSprite(glm::vec2(float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) / 2.f), glm::vec2(1.f, 1.f),
		&bgText_3, &texProgram2);
	background_3->setPosition(background_3->position());

	bgText_4.loadFromFile("images/bg_4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText_4.setMinFilter(GL_NEAREST);
	bgText_4.setMagFilter(GL_NEAREST);

	background_4 = Sprite::createSprite(glm::vec2(float(SCREEN_WIDTH - 1) , float(SCREEN_HEIGHT - 1) / 2.f), glm::vec2(1.f, 1.f),
		&bgText_4, &texProgram2);
	background_4->setPosition(background_4->position());

	bgText_5.loadFromFile("images/bg_5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgText_5.setMinFilter(GL_NEAREST);
	bgText_5.setMagFilter(GL_NEAREST);

	background_5 = Sprite::createSprite(glm::vec2(float(SCREEN_WIDTH - 1)/ 2.f, float(SCREEN_HEIGHT - 1) / 2.f), glm::vec2(1.f, 1.f),
		&bgText_5, &texProgram2);
	background_5->setPosition(background_5->position());
	titlePosition = glm::vec2(740.f, 190.f);
	canContinue = false;
}

CMenuState::~CMenuState()
{
}

CMenuState* CMenuState::GetInstance(CStateManager* pManager)
{
	static CMenuState Instance(pManager);
	return &Instance;
}

void CMenuState::Update(DWORD deltaTime) {
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !up) {
		m_iCurrentSelection -= 1;
		if (m_iCurrentSelection == -1) m_iCurrentSelection = 3;
		SoundEngine->play2D("audio/digi_plink.wav", false);
		Draw();
		up = true;
	}
	else if(!Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
		up = false;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !down) {
		m_iCurrentSelection += 1;
		if (m_iCurrentSelection == 4) m_iCurrentSelection = 0;
		Draw();
		down = true;
		SoundEngine->play2D("audio/digi_plink.wav", false);
	}
	else if(!Game::instance().getSpecialKey(GLUT_KEY_DOWN) ){
		down = false;
	}
	if (Game::instance().getKey(13)) {
		switch (m_iCurrentSelection) {
			case 0:
				CSceneState::GetInstance(m_pStateManager)->resetState();
				ChangeState(CSceneState::GetInstance(m_pStateManager));
				canContinue = true;
				break;
			case 1:
				if (canContinue) {
					CSceneState::GetInstance(m_pStateManager)->EnterScene();
					ChangeState(CSceneState::GetInstance(m_pStateManager));
					
				}
				break;
			case 2:
				ChangeState(CInstructionsState::GetInstance(m_pStateManager));
				break;
			case 3:
				ChangeState(CCreditsState::GetInstance(m_pStateManager));
				break;
		}
		
	}
	if (background_1->position().x < -float(SCREEN_WIDTH - 1) / 2.f) {
		background_1->setPosition(glm::vec2(0.f, 0.f));
	}
	else {
		background_1->setPosition(glm::vec2(background_1->position().x-5.f, 0.f));
	}
	if (background_2->position().x < -float(SCREEN_WIDTH - 1) / 2.f) {
		background_2->setPosition(glm::vec2(0.f, 0.f));
	}
	else {
		background_2->setPosition(glm::vec2(background_2->position().x - 3.f, 0.f));
	}
	if (background_3->position().x < -float(SCREEN_WIDTH - 1) / 2.f) {
		background_3->setPosition(glm::vec2(0.f, 0.f));
	}
	else {
		background_3->setPosition(glm::vec2(background_3->position().x - 1.f, 0.f));
	}
	if (background_4->position().x < -float(SCREEN_WIDTH - 1) / 2.f) {
		background_4->setPosition(glm::vec2(0.f, 0.f));
	}
	else {
		background_4->setPosition(glm::vec2(background_4->position().x - 0.2f, 0.f));
	}
	background_1->update(deltaTime);
	background_2->update(deltaTime);
	background_3->update(deltaTime);
	background_4->update(deltaTime);
	background_5->update(deltaTime);
}

void CMenuState::Draw()
{
	glm::mat4 modelview;
	modelview = glm::mat4(1.0f);
	texProgram2.use();
	texProgram2.setUniformMatrix4f("projection", projection);
	texProgram2.setUniformMatrix4f("modelview", modelview);
	texProgram2.setUniform2f("texCoordDispl", 0.f, 0.f);
	texProgram2.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	background_5->render();
	background_4->render();
	background_3->render();
	background_2->render();
	background_1->render();
	
	texProgram.use();
	
	texProgram.setUniformMatrix4f("projection", projection);
	
	texProgram.setUniformMatrix4f("modelview", modelview);
	
	for (int i = 0; i < opt.size(); i++) {
		if (i == m_iCurrentSelection) {
			texProgram.setUniform4f("color", 0.5f, 0.5, 0.5f, 1.0f);
			texProgram.setUniform4f("arrow_color", 0.f, 0.f, 0.f, 1.0f);
			opt[i].Draw();
		}
		else {
			texProgram.setUniform4f("color", 0.53f, 0.83f, 0.f, 1.f);
			texProgram.setUniform4f("arrow_color", 0.53f, 0.83f, 0.f, 1.0f);
			opt[i].Draw();
		}
	}
	newText.render("Start", glm::vec2(880.f, 350.f), 32, glm::vec4(0.f, 0.f, 0.f, 1.f));
	restartText.render("Continue", glm::vec2(850.f, 500.f), 32, glm::vec4(0.f, 0.f, 0.f, 1.f));
	optionsText.render("Instructions", glm::vec2(820.f, 650.f), 32, glm::vec4(0.f, 0.f, 0.f, 1.f));
	exitText.render("Credits", glm::vec2(870.f, 800.f), 32, glm::vec4(0.f, 0.f, 0.f, 1.f));
	title.render("SISAO", titlePosition, 128, glm::vec4(0.f, 0.f, 0.f, 1.f));
}

void CMenuState::EnterState()
{
	SoundEngine->play2D("audio/Menu.mp3", true);
	Game::instance().keyReleased(13);
}

void CMenuState::LeaveState()
{
	SoundEngine->stopAllSounds();
	Game::instance().keyReleased(13);
}

void CMenuState::SelectionUp()
{

}

void CMenuState::SelectionDown()
{

}

void CMenuState::SelectionChosen()
{
	
}

void CMenuState::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
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

void CMenuState::initTextShaders()
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
	texProgram2.init();
	texProgram2.addShader(vShader);
	texProgram2.addShader(fShader);
	texProgram2.link();
	if (!texProgram2.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram2.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
