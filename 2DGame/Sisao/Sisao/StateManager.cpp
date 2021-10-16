#include "StateManager.h"
#include "GameState.h"

CStateManager::CStateManager() : m_pActiveState(NULL)
{
}

CStateManager::~CStateManager()
{
}

void CStateManager::Update(DWORD dwCurrentTime)
{
	if (m_pActiveState)
		m_pActiveState->Update(dwCurrentTime);
}

void CStateManager::Draw()
{
	if (m_pActiveState)
		m_pActiveState->Draw();
}