#ifndef _INSTRUCTIONSSTATE_H_
#define _INSTRUCTIONSSTATE_H_

#include "GameState.h"
#include "Scene.h"
#include "TextBlock.h"


class CInstructionsState : public CGameState
{
public:
	~CInstructionsState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();
	void Update(DWORD deltaTime);

	static CInstructionsState* GetInstance(CStateManager* pManager);

protected:
	CInstructionsState(CStateManager* pManager);

private:
	// The player went up or down in 
	// the menu
	void initShaders();
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	bool right;
	bool left;

	ShaderProgram texProgram;
	Text moveLeft;
	Text moveRight;
	Text Jump;
	Text specialKeys;
	Text godMode;
	Text removeBarriers;
	Text skipLevel;
	Text goMenu;
	Text goBack;
	Text goNext;
	Text objects;

	glm::mat4 projection;
	vector<TextBlock> opt;
	Text title;

	Texture keyLeftText;
	Sprite* keyLeft;
	Texture keyUpText;
	Sprite* keyUp;
	Texture keyRightText;
	Sprite* keyRight;

	Player* playerLeft;
	Player* playerRight;
	Player* playerUp;
	int page;

	Card* card1;
	Texture leverText;
	Sprite* Lever;

	Texture barrierText;
	Sprite* Barrier;

	Texture boxText;
	Sprite* Box;

	HydraulicPress* hammer;

	Texture transporterText;
	Sprite* Transporter;


};

#endif  // _INSTRUCTIONSSTATE_H_
 