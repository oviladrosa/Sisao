#ifndef _SCENESTATE_H_
#define _SCENESTATE_H_

#include "GameState.h"
#include "Scene.h"
#include "TextBlock.h"

class CSceneState : public CGameState
{
public:
	~CSceneState();

	void Draw();
	void EnterScene();
	void Update(DWORD deltaTime);
	void resetState();
	void initLevel01();
	void initLevel02();
	void initLevel03();
	void initLevel04();
	void initLevel05();
	static CSceneState* GetInstance(CStateManager* pManager);
	void nextLevel();

protected:
	CSceneState(CStateManager* pManager);

private:
	CSceneManager* m_pSceneManager;
	Scene* pointerScene1;
	Scene* pointerScene2;
	Scene* pointerScene3;
	Scene* pointerScene4;
	Scene* pointerScene5;
};

#endif  // _SCENESTATE_H_