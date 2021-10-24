#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <Windows.h>
#include "GameState.h"
#include "SceneState.h"
#include "Scene.h"

//Manages the different states of the game
class CSceneManager
{
public:

	CSceneManager(CStateManager* m_pStateManager);
	~CSceneManager();

	// Switches to another active state.
	void ChangeScene(CSceneState* sNewScene)
	{
		if (m_pActiveScene)
			m_pActiveScene->LeaveScene();
		m_pActiveScene = sNewScene;
		m_pActiveScene->EnterScene();
	}

	// Returns the current active state.
	CSceneState* GetActiveScene() { return m_pActiveScene; }

	// 'Events' function, they are simply redirected to
	// the active state.
	void Update(DWORD dwCurrentTime);
	void Draw();

private:
	// Active State of the game (intro, play, ...)
	CSceneState* m_pActiveScene;
	CStateManager* m_pStateManager;
};
#endif  // _SCENEMANAGER_H_
