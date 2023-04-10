#include "CSky.h"
#define TEXCOORD 0.0f, 855.0f, 380.0f, 225.0f

CSky::CSky(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
}