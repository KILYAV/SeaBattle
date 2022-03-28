#pragma once
#include "framework.h"
#include "Resource.h"
#include "CBase.h"

class CBoard : virtual protected CBase
{
public:
	enum ELevel
	{
		eLevel0,
		eLevel1,
		eLevel2,
		eLevel3,
		eLevel4,
		eLevel5
	};

private:
	UINT   mMaxLevel;
	PUINT  aFleet[eBoardTotal];
	P2UINT aBoard[eBoardTotal];
	P2UINT aCover[eBoardTotal];

private:
	UINT TestLineUp(const SPoint&, const UINT);
	UINT TestLineDown(const SPoint&, const UINT);
	UINT TestLineLeft(const SPoint&, const UINT);
	UINT TestLineRight(const SPoint&, const UINT);

	UINT TestTripletHorizon(const SPoint&, const UINT);
	UINT TestTripletVertical(const SPoint&, const UINT);

	UINT TestDiagonal(const SPoint&, const UINT);

	bool AddCellHorizon(const SPoint&, const UINT, const UINT);
	bool AddCellVertical(const SPoint&, const UINT, const UINT);
	bool CreateShipHorizon(const SPoint&, const UINT, const UINT);
	bool CreateShipVertical(const SPoint&, const UINT, const UINT);

private:
	bool (CBoard::*CreateShipRandom)(const SPoint&, const UINT, const UINT);

protected:
	CBoard();
	~CBoard();

protected:
	UINT CreateShip(const SPoint&, const UINT);
	UINT DeleteShip(const SPoint&, const UINT);

	void RandomOrder(const UINT);
	bool TestLevelShip();

	P3UINT const GetBoard() { return aBoard; };

	bool HitCover(const SPoint&, const UINT);
	void HitDiagonal(const SPoint&, const UINT);
};
