#include "CBlock.h"

#define TEXCOORD 49.0f, 80.0f, 63.0f, 40.0f //テクスチャ座標

CBlock::CBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h*3);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}