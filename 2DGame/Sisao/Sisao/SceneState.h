#ifndef _SCENESTATE_H_
#define _SCENESTATE_H_

#include "GameState.h"
#include "Scene.h"
#include "TextBlock.h"

// Specialization of the CGameState class for 
// the menu state. This displays a menu in which
// the player can start a new game, continue an 
// existing game, see the high-scores or exit the game.
class CSceneState : public CGameState
{
public:
	~CSceneState();

	void Draw();
	void EnterScene();
	void Update(DWORD deltaTime);
	void resetState();

	static CSceneState* GetInstance(CStateManager* pManager);

protected:
	CSceneState(CStateManager* pManager);

private:
	CSceneManager* m_pSceneManager;


};

#endif  // _SCENESTATE_H_