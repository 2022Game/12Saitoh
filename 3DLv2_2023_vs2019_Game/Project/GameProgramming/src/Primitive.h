#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "CVector.h"

namespace Primitive
{
	void DrawLine(const CVector& s, const CVector& e, const CVector4& color, const float width = 1.0f);
	void DrawSector(const CVector& pos, const CVector& rot, const float start, const float end, const float size, const CVector4& color, const int div = 15);
	void DrawCube(const CVector& pos, const CVector& size, const CVector4& color);
	void DrawSphere(const CVector& pos, const float rad, const CVector4& color, const int div = 15);
}

#endif