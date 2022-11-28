#include "CBlock2.h"
#include "CApplication.h"
#define TEXCOORD 0.0f, 48.0f, 128.0f, 80.0f //�e�N�X�`�����W

CBlock2::CBlock2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.1, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}