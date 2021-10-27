#include "GameScene.h"
#include "SceneManager.h"

CGameScene::CGameScene(CSceneManager* pManager)
	: m_pSceneManager(pManager)
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::ChangeScene(CGameScene* pNewState)
{
	m_pSceneManager->ChangeScene(pNewState);
}