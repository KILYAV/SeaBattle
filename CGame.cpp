#include "CGame.h"

CGame::CGame(HINSTANCE hInstance) : CBoard(),
    CDrawing(hInstance, { WndMainProc, WndEnemyProc, WndAlliesProc }, this),
    mHInstance(hInstance)
{
    DrawFull(eAllies);
    DrawFull(eEnemy);
};

bool CGame::ActionEnemy(const LPARAM lParam)
{
    SPoint sPoint = {
        (lParam & 0xFFFF) / GetScale(),
        (lParam >> 0x10) / GetScale()
    };

    if (HitCover(sPoint, eEnemy))
        return false;

    if (eShip == GetBoard()[eEnemy][sPoint.x][sPoint.y])
    {
        DeleteShip(sPoint, eEnemy);
        DrawCell(sPoint, eEnemy, eBlast);
        DrawDiagonal(sPoint, eEnemy);
        HitDiagonal(sPoint, eEnemy);
    }
    else
    {
        DrawCell(sPoint, eEnemy, eSea);
        EnemyFire();
    };
    return true;
};
void CGame::ActionAllies(const LPARAM lParam)
{
    UINT cell;
    SPoint sPoint = {
        (lParam & 0xFFFF) / GetScale(),
        (lParam >> 0x10) / GetScale()
    };
    if (eShip == GetBoard()[eAllies][sPoint.x][sPoint.y])
        cell = DeleteShip(sPoint, eAllies);
    else
        cell = CreateShip(sPoint, eAllies);
    DrawCell(sPoint, eAllies, cell);
}
void CGame::EnemyFire()
{
    SPoint sPoint;
    do {
        RandomPoint(sPoint);
    } while (HitCover(sPoint, eAllies));

    if (eShip == GetBoard()[eAllies][sPoint.x][sPoint.y])
    {
        DeleteShip(sPoint, eAllies);
        DrawCell(sPoint, eAllies, eBlast);
        //DeleteDiagonal(sPoint);
    }
    else
        DrawCell(sPoint, eAllies, eMissle);
};
void CGame::StartCombat()
{
    if (TestLevelShip())
    {
        EnableWindow(GetHWnd()[eEnemy], true);
        EnableWindow(GetHWnd()[eAllies], false);
        RandomOrder(eEnemy);
        MessageBox(nullptr, L"Battle stage", L"Battle stage", MB_OKCANCEL);
    };
};
LRESULT CALLBACK CGame::WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PCGAME pcGame = GetInctance(hWnd);
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_ORDER:
            if (IDOK == MessageBox(hWnd, L"Random fleet order", L"Random fleet order", MB_OKCANCEL))
            {
                pcGame->RandomOrder(eAllies);
                pcGame->DrawFull(eAllies);
            };
             break;
        case IDM_BATTLE:
            pcGame->StartCombat();
            break;
        case IDM_ABOUT:
            DialogBox(pcGame->mHInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        };
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
};
LRESULT CALLBACK CGame::WndEnemyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        GetInctance(hWnd)->ActionEnemy(lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
};
LRESULT CALLBACK CGame::WndAlliesProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        GetInctance(hWnd)->ActionAllies(lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
};

INT_PTR CALLBACK CGame::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
