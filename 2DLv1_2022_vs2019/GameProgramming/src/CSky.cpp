#include "CSky.h"

#define TEXCOORD 108.0f,203.0f,254.0f,213.0f //�e�N�X�`�����W

CSky::CSky(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h );
	Texture(pt, TEXCOORD);
}