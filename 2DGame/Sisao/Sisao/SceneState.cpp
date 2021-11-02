#include "SceneState.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "StateManager.h"
#include <iostream>
#include "Scene.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;



CSceneState::CSceneState(CStateManager* pManager)
	: CGameState(pManager)
{
	m_pSceneManager = new CSceneManager;
	pointerScene1 = new Scene(m_pSceneManager);
	initLevel01();
	pointerScene2 = new Scene(m_pSceneManager);
	initLevel02();
	pointerScene3 = new Scene(m_pSceneManager);
	initLevel03();
	pointerScene4 = new Scene(m_pSceneManager);
	initLevel04();
	pointerScene5 = new Scene(m_pSceneManager);
	initLevel05();
	m_pSceneManager->ChangeScene(pointerScene1);
}

CSceneState::~CSceneState()
{
}

CSceneState* CSceneState::GetInstance(CStateManager* pManager)
{
	static CSceneState Instance(pManager);
	return &Instance;
}

void CSceneState::Update(DWORD deltaTime) {
	m_pSceneManager->Update(deltaTime);
	if (Game::instance().getKey(27)) {
		m_pSceneManager->GetActiveScene()->LeaveScene();
		ChangeState(CMenuState::GetInstance(m_pStateManager));
	}
	if (m_pSceneManager->GetActiveScene()->isFinished()) {
		int next = m_pSceneManager->GetActiveScene()->getNextScene();
		switch (next){
			case 1:
				m_pSceneManager->ChangeScene(pointerScene1);
				break;
			case 2:
				m_pSceneManager->ChangeScene(pointerScene2);
				break;
			case 3:
				m_pSceneManager->ChangeScene(pointerScene3);
				break;
			case 4:
				m_pSceneManager->ChangeScene(pointerScene4);
				break;
			case 5:
				m_pSceneManager->ChangeScene(pointerScene5);
				break;
			default:
				break;
		}
	}
	if (Game::instance().getKey(49)) {
		pointerScene1->Reset();
		m_pSceneManager->ChangeScene(pointerScene1);
	}
	if (Game::instance().getKey(50)) {
		pointerScene2->Reset();
		m_pSceneManager->ChangeScene(pointerScene2);
	}
	if (Game::instance().getKey(51)) {
		pointerScene3->Reset();
		m_pSceneManager->ChangeScene(pointerScene3);
	}
	if (Game::instance().getKey(52)) {
		pointerScene4->Reset();
		m_pSceneManager->ChangeScene(pointerScene4);
	}
	if (Game::instance().getKey(53)) {
		pointerScene5->Reset();
		m_pSceneManager->ChangeScene(pointerScene5);
	}
	
}

void CSceneState::Draw()
{
	m_pSceneManager->Draw();
}

void CSceneState::EnterScene()
{
	m_pSceneManager->GetActiveScene()->EnterScene();
}

void CSceneState::resetState() {
	m_pSceneManager->ChangeScene(pointerScene1);
	m_pSceneManager->Reset();
}

void CSceneState::initLevel01() {
	pointerScene1->setLevel("levels/level01.txt");
	pointerScene1->setNextScene(2);
	pointerScene1->setcard1Postion(glm::vec2(30.f, 5.f));
	pointerScene1->setcard2Postion(glm::vec2(30.f, 10.f));
	pointerScene1->setplayerPostion(glm::ivec2(8, 5));
	pointerScene1->setmirrorplayerPostion(glm::ivec2(8, 15));
	
	pointerScene1->init();
	pointerScene1->addLever(glm::vec2(12, 6));
	pointerScene1->addTransporter(glm::vec2(4, 6), false);
	pointerScene1->addTransporter(glm::vec2(1, 6), false);
	pointerScene1->addTransporter(glm::vec2(4, 9), true);
	pointerScene1->addTransporter(glm::vec2(1, 9), false);
	pointerScene1->addSpike(glm::vec2(19, 3), true);
	pointerScene1->addSpike(glm::vec2(10, 10), false);
	pointerScene1->addHammer(glm::vec2(14, 5), false);
	pointerScene1->addHammer(glm::vec2(14, 9), true);
	pointerScene1->addBox(glm::vec2(8, 6), false);
	pointerScene1->addBox(glm::vec2(9, 9), true);
	pointerScene1->addWall(glm::vec2(28, 6));
	pointerScene1->addWall(glm::vec2(28, 5));
	
}

void CSceneState::initLevel02() {
	pointerScene2->setLevel("levels/level02.txt");
	pointerScene2->setNextScene(3);
	pointerScene2->setcard1Postion(glm::vec2(30.f, 5.f));
	pointerScene2->setcard2Postion(glm::vec2(22.f, 11.f));
	pointerScene2->setplayerPostion(glm::ivec2(8, 5));
	pointerScene2->setmirrorplayerPostion(glm::ivec2(8, 15));
	pointerScene2->init();
}

void CSceneState::initLevel03() {
	pointerScene3->setLevel("levels/level03.txt");
	pointerScene3->setNextScene(4);
	pointerScene3->setcard1Postion(glm::vec2(34.f, 3.f));
	pointerScene3->setcard2Postion(glm::vec2(37.f, 13.f));
	pointerScene3->setplayerPostion(glm::ivec2(14, 3));
	pointerScene3->setmirrorplayerPostion(glm::ivec2(8, 15));
	pointerScene3->init();
}

void CSceneState::initLevel04() {
	pointerScene4->setLevel("levels/level04.txt");
	pointerScene4->setNextScene(5);
	pointerScene4->setcard1Postion(glm::vec2(30.f, 5.f));
	pointerScene4->setcard2Postion(glm::vec2(30.f, 10.f));
	pointerScene4->setplayerPostion(glm::ivec2(8, 5));
	pointerScene4->setmirrorplayerPostion(glm::ivec2(8, 15));
	pointerScene4->init();
}

void CSceneState::initLevel05() {
	pointerScene5->setLevel("levels/level02.txt");
	pointerScene5->setNextScene(1);
	pointerScene5->setcard1Postion(glm::vec2(30.f, 5.f));
	pointerScene5->setcard2Postion(glm::vec2(30.f, 10.f));
	pointerScene5->setplayerPostion(glm::ivec2(8, 5));
	pointerScene5->setmirrorplayerPostion(glm::ivec2(8, 15));
	pointerScene5->init();
}
