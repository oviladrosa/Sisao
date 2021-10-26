#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <Windows.h>


class CSceneManager;
// Base class for the different states
// of the game.
class CGameScene
{
public:
	// Constructor
	CGameScene(CSceneManager* pManager);
	// Destructor
	virtual ~CGameScene();

	// The different 'events' functions. Child classes can 
	// implement the ones in which they are interested in.

	virtual void Update(DWORD) { }
	virtual void Draw() { }
	virtual void Reset() { }

	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterScene() { }
	virtual void LeaveScene() { }

protected:
	// Helper function to switch to a new active state.
	void ChangeScene(CGameScene* pNewState);

	// The state manager.
	CSceneManager* m_pSceneManager;
};

#endif  // _GAMESCENE_H_

