#pragma once
#include "framework.h"
#include "Resource.h"

class CIndex;

using PCIndex = CIndex*;
using Pvoid = void*;


class CIndex {
private:
	static PCIndex start;
	PCIndex next;

	const HWND fMain;
	const HWND fAllies;
	const HWND fEnemy;

	const Pvoid pVoid;

	static UINT Index();

protected:
	CIndex(HWND, HWND, HWND, Pvoid);

public:
	CIndex() = delete;
};
