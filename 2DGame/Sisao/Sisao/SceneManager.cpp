#include "SceneManager.h"
#include "GameScene.h"

CSceneManager::CSceneManager() : m_pActiveScene(NULL)
{
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

void CSceneManager::Reset() {
	m_pActiveScene->Reset();
}