#include "SceneManager.h"
#include "SceneState.h"

CSceneManager::CSceneManager(CStateManager* stateManager) : m_pActiveScene(NULL)
{
	m_pStateManager = stateManager;
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::Update(DWORD dwCurrentTime)
{
	if (m_pActiveScene)
		m_pActiveScene->Update(dwCurrentTime);
}

void CSceneManager::Draw()
{
	if (m_pActiveScene)
		m_pActiveScene->Draw();
}