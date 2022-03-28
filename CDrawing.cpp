#include "CDrawing.h"

const CDrawing::SCell CDrawing::aCell[] = {
	{ CreateSolidBrush(eBlue),  CreatePen(PS_SOLID, 1, eWhite) },
	{ CreateSolidBrush(eWhite), CreatePen(PS_SOLID, 1, eBlue)  },
	{ CreateSolidBrush(eBlace), CreatePen(PS_SOLID, 1, eBlace) },
	{ CreateSolidBrush(eRed),   CreatePen(PS_SOLID, 1, eRed)   },
	{ CreateSolidBrush(eBlue), CreatePen(PS_SOLID, 2, eRed)   }
};

CDrawing::CDrawing(const HINSTANCE hInstance, const EWndProc pWndProc,
	const PCGAME cGame) : CFrame (hInstance, pWndProc, cGame) {};

void CDrawing::DrawFull(const UINT type)
{
	HDC hDC = GetHDC()[type];
	UINT size = GetSizeBoard();
	UINT scale = GetScale();
	P2UINT board = GetBoard()[type];

	for (UINT cell = 0; cell < eCellTotal; cell++)
		DRAWCELL(board, hDC, size, scale, cell);
};
void CDrawing::DrawCell(const SPoint& point, const UINT board, const UINT type)
{
	HDC hDC = GetHDC()[board];
	UINT scale = GetScale();

	SELECTSCELL(hDC, type);
	Rectangle(hDC, point.x * scale, point.y * scale,
		(point.x + 1) * scale, (point.y + 1) * scale);
};
void CDrawing::DrawDiagonal(const SPoint& sPoint, const UINT board)
{
	HDC  hDC = GetHDC()[board];
	UINT size = GetSizeBoard() - 1;
	UINT scale = GetScale();
	UINT scaleX = scale * sPoint.x;
	UINT scaleY = scale * sPoint.y;

	SELECTSCELL(hDC, eSea);

	if (sPoint.x && sPoint.y)
		Rectangle(hDC, scaleX - scale, scaleY - scale, scaleX, scaleY);

	scaleY += scale;
	if (sPoint.x && sPoint.y < size)
		Rectangle(hDC, scaleX - scale, scaleY, scaleX, scaleY + scale);

	scaleX += scale;
	if (sPoint.x < size && sPoint.y)
		Rectangle(hDC, scaleX, scaleY, scaleX + scale, scaleY + scale);

	scaleY -= scale;
	if (sPoint.x < size && sPoint.y < size)
		Rectangle(hDC, scaleX, scaleY - scale, scaleX + scale, scaleY);
}
