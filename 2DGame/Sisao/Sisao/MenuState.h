#ifndef _MENUSTATE_H_
#define _MENUSTATE_H_

#include "GameState.h"
#include "Scene.h"
#include "TextBlock.h"

// Specialization of the CGameState class for 
// the menu state. This displays a menu in which
// the player can start a new game, continue an 
// existing game, see the high-scores or exit the game.
class CMenuState : public CGameState
{
public:
	~CMenuState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();
	void Update(DWORD deltaTime);

	static CMenuState* GetInstance(CStateManager* pManager);

protected:
	CMenuState(CStateManager* pManager);

private:
	// The player went up or down in 
	// the menu
	void initShaders();
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	
	// Index of the current selected menu item
	int m_iCurrentSelection;

	// A pointer to the current active game (if any).
	Scene* m_pCurrentGame;
	ShaderProgram texProgram;
	TextBlock *newBlock;
	TextBlock *restartBlock;
	TextBlock *optionsBlock;
	TextBlock *exitBlock;
	glm::mat4 projection;
	vector<TextBlock> opt;
	bool up;
	bool down;

};

#endif  // _MENUSTATE_H_