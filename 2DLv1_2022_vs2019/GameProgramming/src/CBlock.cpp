#include "CBlock.h"

#define TEXCOORD 0.0f, 48.0f, 128.0f, 80.0f //テクスチャ座標

CBlock::CBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.1, h*1.1);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}