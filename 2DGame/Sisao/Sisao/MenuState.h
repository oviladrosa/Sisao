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
	void initTextShaders();
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	
	// Index of the current selected menu item
	int m_iCurrentSelection;

	// A pointer to the current active game (if any).
	Scene* m_pCurrentGame;
	ShaderProgram texProgram;
	ShaderProgram texProgram2;
	TextBlock *newBlock;
	Text newText;
	TextBlock *restartBlock;
	Text restartText;
	TextBlock *optionsBlock;
	Text optionsText;
	TextBlock *exitBlock;
	Text exitText;
	glm::mat4 projection;
	vector<TextBlock> opt;
	Text title;
	bool up;
	bool down;
	Texture bgText_1;
	Sprite* background_1;
	Texture bgText_2;
	Sprite* background_2;
	Texture bgText_3;
	Sprite* background_3;
	Texture bgText_4;
	Sprite* background_4;
	Texture bgText_5;
	Sprite* background_5;
	glm::vec2 titlePosition;

};

#endif  // _MENUSTATE_H_