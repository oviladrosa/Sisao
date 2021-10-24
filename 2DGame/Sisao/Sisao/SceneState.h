#ifndef _SCENESTATE_H_
#define _SCENESTATE_H_

#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameState.h"
#include "SceneManager.h"

class CSceneManager;

class CSceneState : public CGameState
{

public:


	~CSceneState();

	virtual void Update(DWORD);
	virtual void Draw();

	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterScene();
	virtual void LeaveScene();

	static CSceneState* GetInstance(CStateManager* sManager);

protected:
	// Helper function to switch to a new active state.
	CSceneState(CStateManager* stateManager);

	void ChangeScene(CSceneState* sNewScene);

	// The state manager.
	CSceneManager* m_pSceneManager;
	CStateManager* m_pStateManager;

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
};

#endif  // _SCENESTATE_H_