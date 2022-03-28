#pragma once
#include "framework.h"
#include "Resource.h"

class CGame;
using PCGAME = CGame*;

class CList
{
public:
	using PHWND = HWND*;
	using PCLIST = CList*;

public:
	using PFTEST = bool (*)(const PHWND, const HWND);

private:
	static PCLIST stStart;
	static PCLIST stEnd;
	static PFTEST TestContent;

private:
	PHWND  paHWnd;
	PCGAME pcGame;
	PCLIST  mPrey;
	PCLIST  mNext;

protected:
	CList(const PHWND, const PFTEST, const PCGAME);
	~CList();

	static const PCGAME GetInctance(const HWND);

public:
	CList() = delete;
};
