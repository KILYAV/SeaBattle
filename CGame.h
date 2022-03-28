#pragma once
#include "framework.h"
#include "Resource.h"

#include "CList.h"
#include "CFrame.h"
#include "CDrawing.h"
#include "CBoard.h"

class CGame : private CBoard, CDrawing
{
private:
	HINSTANCE mHInstance;

private:
	static LRESULT CALLBACK WndMainProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndEnemyProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndAlliesProc(HWND, UINT, WPARAM, LPARAM);
	
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

private:
	bool ActionEnemy(const LPARAM);
	void ActionAllies(const LPARAM);
	void StartCombat();
	void EnemyFire();

public:
	CGame(const HINSTANCE);
};
