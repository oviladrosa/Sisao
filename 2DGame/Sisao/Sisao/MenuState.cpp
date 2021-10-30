#include "MenuState.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "StateManager.h"
#include "SceneState.h"
#include "InstructionsState.h"
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
	if (!title.init("fonts/Tourney-BoldItalic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 2.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	m_iCurrentSelection = 0;
	up = false;
	down = false;
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
				break;
			case 1:
				CSceneState::GetInstance(m_pStateManager)->EnterScene();
				ChangeState(CSceneState::GetInstance(m_pStateManager));
				break;
			case 2:
				ChangeState(CInstructionsState::GetInstance(m_pStateManager));
				break;
		}
		
	}
}

void CMenuState::Draw()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	modelview = glm::mat4(1.0f);
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
	exitText.render("Exit", glm::vec2(900.f, 800.f), 32, glm::vec4(0.f, 0.f, 0.f, 1.f));
	title.render("SISAO", glm::vec2(740.f, 190.f), 128, glm::vec4(0.53f, 0.83f, 0.f, 1.f));
}

void CMenuState::EnterState()
{
	
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
