#include "CGoal.h"

#define TEXCOORD8 230.0f, 252.0f, 255.0f, 113.0f //テクスチャ座標

CGoal::CGoal(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w * 1.1, h * 7);
	Texture(pt, TEXCOORD8);
	mTag = ETag::EGOAL;
}