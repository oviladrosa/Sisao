#pragma once
#ifndef _CREDITSSTATE_H_
#define _CREDITSSTATE_H_

#include "GameState.h"
#include "Scene.h"
#include "TextBlock.h"


class CCreditsState : public CGameState
{
public:
	~CCreditsState();

	void OnKeyDown(WPARAM wKey);
	void Draw();
	void EnterState();
	void LeaveState();
	void Update(DWORD deltaTime);

	static CCreditsState* GetInstance(CStateManager* pManager);

protected:
	CCreditsState(CStateManager* pManager);

private:
	// The player went up or down in 
	// the menu
	void initShaders();
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();


	ShaderProgram texProgram;

	Text Credits;

	float counter;
	TextBlock* bg;


	glm::mat4 projection;
	
	


};

#endif  // _CREDITSSTATE_H_
