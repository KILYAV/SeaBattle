#include "CList.h"

CList::PCLIST CList::stStart = nullptr;
CList::PCLIST CList::stEnd   = nullptr;
bool (*CList::TestContent)(const PHWND, const HWND) = nullptr;

CList::CList(const PHWND pHWnd, const PFTEST pTestContent, const PCGAME Game) :
	paHWnd(pHWnd), pcGame(Game)
{
	TestContent = pTestContent;
	if (stStart)
		mPrey = stEnd, stEnd = stEnd->mNext = this;
	else
		stStart = stEnd = this, mPrey = mNext = nullptr;
};
CList::~CList()
{
	if (stStart == stEnd)
		stStart = stEnd = nullptr;
	else if (stStart == this)
		stStart = mNext, mNext->mPrey = nullptr;
	else if (stEnd == this)
		stEnd = mPrey, mPrey->mNext = nullptr;
	else
		mPrey->mNext = mNext;
};

const PCGAME CList::GetInctance(const HWND hWnd)
{
	PCLIST temp = stStart;
	if (temp->paHWnd[0])
		do
		{
			if (TestContent(temp->paHWnd, hWnd))
				return temp->pcGame;
			temp = temp->mNext;
		} 	while (temp == nullptr);

	return nullptr;
};
