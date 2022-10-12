#include "CDOKAN.h"
#define TEXCOORD 103.0f, 153.0f, 120.0f, 56.0f //テクスチャ座標

CDOKAN::CDOKAN(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*2, h*3.0);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}
