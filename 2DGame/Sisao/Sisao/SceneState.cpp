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
	m_pSceneManager->ChangeScene(Scene::GetInstance(m_pSceneManager));
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
	m_pSceneManager->ChangeScene(Scene::GetInstance(m_pSceneManager));
	m_pSceneManager->Reset();
}


