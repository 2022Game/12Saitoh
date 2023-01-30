#include "CNclimbBlock.h"

#define TEXCOORD 49.0f, 80.0f, 63.0f, 40.0f //�e�N�X�`�����W

CNclimbBlock::CNclimbBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h*7);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}