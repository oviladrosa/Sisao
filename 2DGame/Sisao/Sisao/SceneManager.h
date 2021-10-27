#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "GameScene.h"

// Manages the different states of the game.
class CSceneManager
{
public:
	// Default constructor
	CSceneManager();
	// Default destructor
	~CSceneManager();

	// Switches to another active state.
	void ChangeScene(CGameScene* pNewScene)
	{
			if (m_pActiveScene)
				m_pActiveScene->LeaveScene();
			m_pActiveScene = pNewScene;
			m_pActiveScene->EnterScene();
	}

	// Returns the current active state.
	CGameScene* GetActiveScene() { return m_pActiveScene; }

	// 'Events' function, they are simply redirected to
	// the active state.
	void Update(DWORD dwCurrentTime);
	void Draw();
	void Reset();

private:
	// Active State of the game (intro, play, ...)
	CGameScene* m_pActiveScene;
};

#endif  // _SCENEMANAGER_H_

