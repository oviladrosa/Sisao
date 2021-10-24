#include "SceneState.h"
#include "GameState.h"
#include "SceneManager.h"
#include "StateManager.h"

CSceneState::CSceneState(CStateManager* stateManager) : CGameState(stateManager)
{
	m_pSceneManager = new CSceneManager(m_pStateManager);
	m_pSceneManager->ChangeScene(Scene::GetInstance(m_pSceneManager));
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
	m_pSceneManager->ChangeState(sNewScene);
}

void CSceneState::Update(DWORD deltaTime)
{
	
}

void CSceneState::Draw()
{

}
