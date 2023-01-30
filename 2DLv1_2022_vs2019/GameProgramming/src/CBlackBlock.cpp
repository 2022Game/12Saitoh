#include "CBlackBlock.h"

#define TEXCOORD 52.0f, 89.0f, 183.0f, 132.0f //テクスチャ座標

CBlackBlock::CBlackBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBACK;
}