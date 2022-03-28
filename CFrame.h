#pragma once
#include "framework.h"
#include "Resource.h"
// Content 
#include "CBase.h"
#include "CList.h"

class CFrame : public CList, virtual protected CBase {
public:
	enum EScale
	{
		eScale20 = 20,
		eScale40 = 40,
		eScale80 = 80
	};

public:
	struct EWndProc
	{
		WNDPROC sMain;
		WNDPROC sEnemy;
		WNDPROC sAllies;
	};

private:
	static WNDCLASSEXW wcex;

private:
	UINT mScale;
	UINT mSizeFrame;

private:
	static bool TestContent(const PHWND, const HWND);

private:
	void CreateMainFrame(const WNDPROC);
	void CreateEnemyFrame(const WNDPROC);
	void CreateAlliesFrame(const WNDPROC);

private:
	HWND aHWnd[eFrameTotal];
	HDC  aHDC[eFrameTotal - 1];

protected:
	CFrame(const HINSTANCE, const EWndProc, const PCGAME);

	PHDC  const GetHDC() { return aHDC; };
	PHWND const GetHWnd() { return aHWnd; };
	UINT  const GetScale() { return mScale; };

public:
	CFrame() = delete;
};
