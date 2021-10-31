#include "CreditsState.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "StateManager.h"
#include "SceneState.h"
#include <iostream>
#include "Scene.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;
#define SCREEN_X 0
#define SCREEN_Y 0

ISoundEngine* SoundEngine4 = createIrrKlangDevice();

CCreditsState::CCreditsState(CStateManager* pManager)
	: CGameState(pManager)
{
	initShaders();
	//glClearColor(0.8f, 1.f, 0.8f, 1.0f);


	if (!Credits.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	counter = 0.f;

}

CCreditsState::~CCreditsState()
{
}

CCreditsState* CCreditsState::GetInstance(CStateManager* pManager)
{
	static CCreditsState Instance(pManager);
	return &Instance;
}

void CCreditsState::Update(DWORD deltaTime) {
	counter += 0.40;
	if (counter > 3100.f) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}
	if (Game::instance().getKey(13) && counter > 10.f) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}
}

void CCreditsState::Draw()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	Credits.render("SISAO", glm::vec2(860.f, 210.f-counter),64, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("VJ-2D", glm::vec2(910.f, 240.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Productors -", glm::vec2(850.f, 320.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 400.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 480.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Art Director -", glm::vec2(840.f, 650.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 730.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Technical Director -", glm::vec2(790.f, 900.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 980.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Sound Director -", glm::vec2(820.f, 1150.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 1230.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Game Designers -", glm::vec2(820.f, 1400.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 1480.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 1550.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Animators -", glm::vec2(860.f, 1720.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 1800.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 1880.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Developers -", glm::vec2(840.f, 2050.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 2130.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 2210.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Testers -", glm::vec2(880.f, 2380.f - counter), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Daniel Vega", glm::vec2(880.f, 2460.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Oriol Viladrosa", glm::vec2(860.f, 2520.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("- Special Thanks -", glm::vec2(830.f, 2690.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Alejandro Rios", glm::vec2(860.f, 2770.f - counter), 28, glm::vec4(1.f, 1.f, 1.f, 1.f));
	Credits.render("Thank you for playing!", glm::vec2(600.f, 3000.f - counter), 64, glm::vec4(1.f, 1.f, 1.f, 1.f));
	if (counter > 10.f) {
		Credits.render("Press ENTER to skip credits", glm::vec2(1600.f, 980.f), 18, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
}

void CCreditsState::EnterState()
{
	counter = 0.f;
}

void CCreditsState::SelectionUp()
{

}

void CCreditsState::SelectionDown()
{

}

void CCreditsState::SelectionChosen()
{

}

void CCreditsState::initShaders()
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
