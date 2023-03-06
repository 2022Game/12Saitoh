#include"CBack.h"
#include"CApplication.h"

#define TEXCOORD 0, 1115, 581, 0 //テクスチャマッピング
//コンストラクタ
CBack::CBack(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w , h);
	Texture(pt, TEXCOORD);
}