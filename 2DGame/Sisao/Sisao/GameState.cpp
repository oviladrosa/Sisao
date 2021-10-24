#include "GameState.h"
#include "StateManager.h"

CGameState::CGameState(CStateManager* pManager)
	: m_pStateManager(pManager)
{
}

CGameState::CGameState(CSceneManager* sManager)
	: m_pSceneManager(sManager)
{
}

CGameState::~CGameState()
{
}

void CGameState::ChangeState(CGameState* pNewState)
{
	m_pStateManager->ChangeState(pNewState);
}