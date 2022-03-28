#include "CBoard.h"

CBoard::CBoard() : CBase(), mMaxLevel(eLevel4)
{
    UINT size = GetSizeBoard();
    for (UINT type = 0; type < eBoardTotal; type++)
    {
        P2UINT board = aBoard[type] = new PUINT[size];
        P2UINT cover = aCover[type] = new PUINT[size];
        for (UINT index = 0; index < size; index++)
        {
            PUINT lineBoard = board[index] = new UINT[size];
            PUINT linecover = cover[index] = new UINT[size];
            for (UINT index = 0; index < size; index++)
            {
                lineBoard[index] = type;
                linecover[index] = eSky;
            }
        }
        PUINT fleet = aFleet[type] = new UINT[mMaxLevel + 1];
        for (UINT level = mMaxLevel; level > 0; level--)
            fleet[level] = 0;
    }
}
CBoard::~CBoard()
{
    UINT size = GetSizeBoard();
    for (UINT type = 0; type < eBoardTotal; type++)
    {
        for (UINT index = 0; index < size; index++)
        {
            delete[] aBoard[type][index];
            delete[] aCover[type][index];
        }
        delete[] aFleet[type];
        delete[] aBoard[type];
        delete[] aCover[type];
    };

};

UINT CBoard::CreateShip(const SPoint& sPoint, const UINT type)
{
    if (TestDiagonal(sPoint, type))
        return eSea;
    
    UINT level[eDirectTotal];

    level[eUp] = TestLineUp(sPoint, type);
    level[eDown] = TestLineDown(sPoint, type);
    level[eLeft] = TestLineLeft(sPoint, type);
    level[eRight] = TestLineRight(sPoint, type);
    level[eCenter] = level[eUp] + level[eDown] + level[eLeft] + level[eRight] + 1;

    if (level[eCenter] > mMaxLevel)
        return eSea;

    aFleet[type][level[eUp]]--;
    aFleet[type][level[eDown]]--;
    aFleet[type][level[eLeft]]--;
    aFleet[type][level[eRight]]--;
    aFleet[type][level[eCenter]]++;

    return aBoard[type][sPoint.x][sPoint.y] = eShip;
};
UINT CBoard::DeleteShip(const SPoint& sPoint, const UINT type)
{
    UINT level[eDirectTotal];

    level[eUp] = TestLineUp(sPoint, type);
    level[eDown] = TestLineDown(sPoint, type);
    level[eLeft] = TestLineLeft(sPoint, type);
    level[eRight] = TestLineRight(sPoint, type);
    level[eCenter] = level[eUp] + level[eDown] + level[eLeft] + level[eRight] + 1;

    aFleet[type][level[eUp]]++;
    aFleet[type][level[eDown]]++;
    aFleet[type][level[eLeft]]++;
    aFleet[type][level[eRight]]++;
    aFleet[type][level[eCenter]]--;

    return aBoard[type][sPoint.x][sPoint.y] = eSea;
};

void CBoard::RandomOrder(const UINT type)
{
    UINT size = GetSizeBoard();

    P2UINT board = aBoard[type];
    for (UINT index = 0; index < size; index++)
    {
        PUINT boardX = board[index];
        for (UINT index = 0; index < size; index++)
            boardX[index] = eSea;
    }

    PUINT fleet = aFleet[type];
    for (UINT level = mMaxLevel; level > 0; level--)
        fleet[level] = mMaxLevel - level + 1;

    UINT max;
    SPoint sPoint;
    for (UINT level = mMaxLevel; level > 0; level--)
    {
        max = size - level;
        for (UINT number = 0; number < aFleet[type][level]; number++)
        {
            do
            {
                sPoint = { RandomUINT(max), RandomUINT(max) };

                if (RandomUINT(1) & 1)
                    CreateShipRandom = &CBoard::CreateShipHorizon;
                else
                    CreateShipRandom = &CBoard::CreateShipVertical;
            }
            while (!(this->*CreateShipRandom)(sPoint, type, level));
        };
    };
 };
bool CBoard::TestLevelShip()
{
    static const wchar_t* levelError[][2] = {
        L"Too small ships of the 1-first level",
        L"Too many ships of the 1-first level",
        L"Too small ships of the 2-second level",
        L"Too many ships of the 2-second level",
        L"Too small ships of the 3-fhird level",
        L"Too many ships of the 3-fhird level",
        L"Too small ships of the 4-fourth level",
        L"Too many ships of the 4-fourth level"
    };

    for (UINT level = mMaxLevel, number = 1; level > 0; level--, number++)
        if (aFleet[eAllies][level] < number)
        {
            MessageBox(nullptr, levelError[level - 1][0], L"Battle stage", MB_OK);
            return false;
        }
        else if (aFleet[eAllies][level] > number)
        {
            MessageBox(nullptr, levelError[level - 1][1], L"Battle stage", MB_OK);
            return false;
        };
    return true;
};

bool CBoard::AddCellHorizon(const SPoint& sPoint, const UINT type, const UINT level)
{
    if (TestTripletHorizon(sPoint, type))
        return false;

    if (level)
        if (!AddCellHorizon({ sPoint.x, sPoint.y + 1 }, type, level - 1))
            return false;

    aBoard[type][sPoint.x][sPoint.y] = eShip;
    return true;
}
bool CBoard::AddCellVertical(const SPoint& sPoint, const UINT type, const UINT level)
{
    if (TestTripletVertical(sPoint, type))
        return false;

    if (level)
        if (!AddCellVertical({ sPoint.x + 1, sPoint.y }, type, level - 1))
            return false;

    aBoard[type][sPoint.x][sPoint.y] = eShip;
    return true;
}
bool CBoard::CreateShipHorizon(const SPoint& sPoint, const UINT type, const UINT level)
{
    if (sPoint.y)
        if (TestTripletHorizon({ sPoint.x, sPoint.y - 1 }, type))
            return false;

    if (sPoint.y < GetSizeBoard())
        if (TestTripletHorizon({ sPoint.x, sPoint.y + level }, type))
            return false;

    if (!AddCellHorizon(sPoint, type, level - 1))
        return false;

    return true;
};
bool CBoard::CreateShipVertical(const SPoint& sPoint, const UINT type, const UINT level)
{
    if (sPoint.x)
        if (TestTripletVertical({ sPoint.x - 1, sPoint.y }, type))
            return false;

    if (sPoint.x < GetSizeBoard())
        if (TestTripletVertical({ sPoint.x + level, sPoint.y }, type))
            return false;
    
    if (!AddCellVertical(sPoint, type, level - 1))
        return false;

    return true;
};

UINT CBoard::TestLineUp(const SPoint& sPoint, const UINT type)
{
    if (sPoint.y)
        if (eShip == aBoard[type][sPoint.x][sPoint.y - 1])
            return eLevel1 + TestLineUp({ sPoint.x, sPoint.y - 1 }, type);
    return eLevel0;
};
UINT CBoard::TestLineDown(const SPoint& sPoint, const UINT type)
{
    if (sPoint.y < (GetSizeBoard() - 1))
        if (eShip == aBoard[type][sPoint.x][sPoint.y + 1])
            return eLevel1 + TestLineDown({ sPoint.x, sPoint.y + 1 }, type);
    return eLevel0;
};
UINT CBoard::TestLineLeft(const SPoint& sPoint, const UINT type)
{
    if (sPoint.x)
        if (eShip == aBoard[type][sPoint.x - 1][sPoint.y])
            return eLevel1 + TestLineLeft({ sPoint.x - 1, sPoint.y }, type);
    return eLevel0;
};
UINT CBoard::TestLineRight(const SPoint& sPoint, const UINT type)
{
    if (sPoint.x < (GetSizeBoard() - 1))
        if (eShip == aBoard[type][sPoint.x + 1][sPoint.y])
            return eLevel1 + TestLineRight({ sPoint.x + 1, sPoint.y }, type);
    return eLevel0;
};

UINT CBoard::TestTripletHorizon(const SPoint& sPoint, const UINT type)
{
    UINT result = aBoard[type][sPoint.x][sPoint.y];
    if (sPoint.x)
        result += aBoard[type][sPoint.x - 1][sPoint.y];
    if (sPoint.x < (GetSizeBoard() - 1))
        result += aBoard[type][sPoint.x + 1][sPoint.y];
    
    return result;
};
UINT CBoard::TestTripletVertical(const SPoint& sPoint, const UINT type)
{
    UINT result = aBoard[type][sPoint.x][sPoint.y];

    if (sPoint.y)
        result += aBoard[type][sPoint.x][sPoint.y - 1];
    if (sPoint.y < (GetSizeBoard() - 1))
        result += aBoard[type][sPoint.x][sPoint.y + 1];

    return result;
};

UINT CBoard::TestDiagonal(const SPoint& sPoint, const UINT type)
{
    UINT size = GetSizeBoard() - 1;
    UINT result = 0;
    P2UINT board = aBoard[type];

    if (sPoint.x && sPoint.y)
        result += board[sPoint.x - 1][sPoint.y - 1];
    if (sPoint.x && sPoint.y < size)
        result += board[sPoint.x - 1][sPoint.y + 1];
    if (sPoint.x < size && sPoint.y)
        result += board[sPoint.x + 1][sPoint.y - 1];
    if (sPoint.x < size && sPoint.y < size)
        result += board[sPoint.x + 1][sPoint.y + 1];

    return result;
};

bool CBoard::HitCover(const SPoint& sPoint, const UINT type)
{
    if (aCover[type][sPoint.x][sPoint.y])
        return (aCover[type][sPoint.x][sPoint.y] = eSea);
    return true;
};
void CBoard::HitDiagonal(const SPoint& sPoint, const UINT type)
{
    UINT size = GetSizeBoard() - 1;
    P2UINT cover = aCover[type];

    if (sPoint.x && sPoint.y)
        cover[sPoint.x - 1][sPoint.y - 1] = eSea;
    if (sPoint.x && sPoint.y < size)
        cover[sPoint.x - 1][sPoint.y + 1] = eSea;
    if (sPoint.x < size && sPoint.y)
        cover[sPoint.x + 1][sPoint.y - 1] = eSea;
    if (sPoint.x < size && sPoint.y < size)
        cover[sPoint.x + 1][sPoint.y + 1] = eSea;
}
