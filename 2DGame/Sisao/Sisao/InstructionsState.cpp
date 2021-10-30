#include "InstructionsState.h"
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

ISoundEngine* SoundEngine3 = createIrrKlangDevice();

CInstructionsState::CInstructionsState(CStateManager* pManager)
	: CGameState(pManager)
{
	initShaders();
	//glClearColor(0.8f, 1.f, 0.8f, 1.0f);


	if (!moveRight.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!moveLeft.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!Jump.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!specialKeys.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!title.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!godMode.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!removeBarriers.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!skipLevel.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!goMenu.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!goBack.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!goNext.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	if (!objects.init("fonts/Tourney-Black.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 2.f, float(SCREEN_HEIGHT - 1) / 2.f, 0.f);
	keyLeftText.loadFromFile("images/key_left.png", TEXTURE_PIXEL_FORMAT_RGBA);
	keyLeftText.setMinFilter(GL_NEAREST);
	keyLeftText.setMagFilter(GL_NEAREST);
	
	keyLeft = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f),&keyLeftText, &texProgram);
	keyLeft->setPosition(glm::vec2(450.f,140.f));

	keyUpText.loadFromFile("images/key_up.png", TEXTURE_PIXEL_FORMAT_RGBA);
	keyUpText.setMinFilter(GL_NEAREST);
	keyUpText.setMagFilter(GL_NEAREST);

	keyUp = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f), &keyUpText, &texProgram);
	keyUp->setPosition(glm::vec2(450.f, 240.f));

	keyRightText.loadFromFile("images/key_right.png", TEXTURE_PIXEL_FORMAT_RGBA);
	keyRightText.setMinFilter(GL_NEAREST);
	keyRightText.setMagFilter(GL_NEAREST);

	keyRight = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f), &keyRightText, &texProgram);
	keyRight->setPosition(glm::vec2(450.f, 190.f));

	playerLeft = new Player();
	playerLeft->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	playerLeft->setPosition(glm::vec2(650.f, 140.f));
	playerLeft->forceAnimation(0);
	playerLeft->changeDemo(true);

	playerRight = new Player();
	playerRight->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	playerRight->setPosition(glm::vec2(650.f, 190.f));
	playerRight->forceAnimation(1);
	playerRight->changeDemo(true);

	playerUp = new Player();
	playerUp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	playerUp->setPosition(glm::vec2(650.f, 240.f));
	playerUp->forceAnimation(2);
	playerUp->changeDemo(true);
	page = 0;

	card1 = new Card();
	card1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	card1->setPosition(glm::vec2(580.f, 125.f));

	leverText.loadFromFile("images/Lever.png", TEXTURE_PIXEL_FORMAT_RGBA);
	leverText.setMinFilter(GL_NEAREST);
	leverText.setMagFilter(GL_NEAREST);

	Lever = Sprite::createSprite(glm::vec2(28.f, 28.f), glm::vec2(1.f, 1.f), &leverText, &texProgram);
	Lever->setPosition(glm::vec2(575.f, 150.f));

	barrierText.loadFromFile("images/Lever.png", TEXTURE_PIXEL_FORMAT_RGBA);
	barrierText.setMinFilter(GL_NEAREST);
	barrierText.setMagFilter(GL_NEAREST);

	Barrier = Sprite::createSprite(glm::vec2(28.f, 28.f), glm::vec2(1.f, 1.f), &leverText, &texProgram);
	Barrier->setPosition(glm::vec2(575.f, 150.f));

}

CInstructionsState::~CInstructionsState()
{
}

CInstructionsState* CInstructionsState::GetInstance(CStateManager* pManager)
{
	static CInstructionsState Instance(pManager);
	return &Instance;
}

void CInstructionsState::Update(DWORD deltaTime) {
	playerLeft->update(deltaTime);
	playerRight->update(deltaTime);
	playerUp->update(deltaTime);
	card1->update(deltaTime);
	if (Game::instance().getKey(27)) {
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		if (page < 1) {
			page++;
		}

	}
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		if (page > 0) {
			page--;
		}
	}
}

void CInstructionsState::Draw()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	if (page == 0) {
		keyLeft->render();
		keyUp->render();
		keyRight->render();

		playerLeft->render();
		playerRight->render();
		playerUp->render();
		moveLeft.render("Move left", glm::vec2(480.f, 300.f), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
		moveRight.render("Move right", glm::vec2(480.f, 400.f), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
		Jump.render("Jump", glm::vec2(480.f, 500.f), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
		specialKeys.render("Special keys", glm::vec2(810.f, 650.f), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
		godMode.render("God Mode					'G'", glm::vec2(600.f, 720.f), 24, glm::vec4(1.f, 1.f, 1.f, 1.f));
		removeBarriers.render("Remove Barriers				'R'", glm::vec2(600.f, 790.f), 24, glm::vec4(1.f, 1.f, 1.f, 1.f));
		skipLevel.render("Skip Level				'1','2','3','4' and '5'", glm::vec2(600.f, 860.f), 24, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
	
	if (page == 1) {
		card1->render();
		Lever->render();
		objects.render("Objects", glm::vec2(860.f, 210.f), 32, glm::vec4(1.f, 1.f, 1.f, 1.f));
		objects.render("Card (goal)", glm::vec2(680.f, 270.f), 26, glm::vec4(1.f, 1.f, 1.f, 1.f));
		objects.render("Lever", glm::vec2(680.f, 340.f), 26, glm::vec4(1.f, 1.f, 1.f, 1.f));
		objects.render("Barrier", glm::vec2(680.f, 410.f), 26, glm::vec4(1.f, 1.f, 1.f, 1.f));
		objects.render("Box", glm::vec2(680.f, 480.f), 26, glm::vec4(1.f, 1.f, 1.f, 1.f));

	}
	
	title.render("INSTRUCTIONS", glm::vec2(710.f, 150.f), 64, glm::vec4(1.f, 1.f, 1.f, 1.f));
	
	goMenu.render("Press ESC to return to the Main Menu", glm::vec2(1570.f, 990.f), 16, glm::vec4(1.f, 1.f, 1.f, 1.f));
	
	if (page > 0) {
		goBack.render("<", glm::vec2(10.f, 500.f), 64, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
	if (page < 1) {
		goNext.render(">", glm::vec2(1880.f, 500.f), 64, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
	
}

void CInstructionsState::EnterState()
{

}

void CInstructionsState::SelectionUp()
{

}

void CInstructionsState::SelectionDown()
{

}

void CInstructionsState::SelectionChosen()
{

}

void CInstructionsState::initShaders()
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
