#include "MenuState.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "StateManager.h"
#include <iostream>
#include "Scene.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>



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
	}
	else if(!Game::instance().getSpecialKey(GLUT_KEY_DOWN) ){
		down = false;
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
			opt[i].Draw();
		}
		else {
			texProgram.setUniform4f("color", 0.5f, 0.5, 1.f, 1.0f);
			opt[i].Draw();
		}
	}
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
