#include "CBase.h"

UINT CBase::RandomUINT(const UINT max)
{
	time_t mTime = 0;

	while (mTime < max)
		time(&mTime);

	UINT random = static_cast<UINT>(mTime) + rand();
	random = random - random / max * max;

	return random;
}
void CBase::RandomPoint(SPoint& sPoint)
{
	sPoint = {
		RandomUINT(mSizeBoard),
		RandomUINT(mSizeBoard) };
}