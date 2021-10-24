#include "SceneState.h"

CSceneState::CSceneState(CStateManager* stateManager) : CGameState(stateManager)
{
	m_pSceneManager = new CSceneManager(m_pStateManager);
	//no hace falta este change scene porque el state manager le pasa al scene manager el estado actual
	//m_pSceneManager->ChangeScene(Scene::GetInstance(m_pSceneManager));
}

CSceneState::~CSceneState()
{
}

CSceneState* CSceneState::GetInstance(CStateManager* stateManager)
{
	static CSceneState Instance(stateManager);
	return &Instance;
}

void CSceneState::ChangeScene(CSceneState* sNewScene)
{
	m_pSceneManager->ChangeScene(sNewScene);
}

void CSceneState::Update(DWORD deltaTime)
{
	
}

void CSceneState::Draw()
{

}