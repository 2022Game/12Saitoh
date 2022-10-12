#include "CPoint.h"

CPoint::CPoint(float x, float y, float w, float h, ETag t)
{
	Set(x, y, w*2, h);
	mTag = t;
}
