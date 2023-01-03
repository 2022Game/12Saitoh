#include "CBlock2.h"
#include "CApplication.h"
#define TEXCOORD 97.0f, 111.0f, 47.0f, 32.0f //テクスチャ座標

CBlock2::CBlock2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.1, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}