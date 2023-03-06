#include"CEnemy.h"
#include"CApplication.h"

#define TEXCOORD 165, 351, 235, 127 //テクスチャマッピング

//コンストラクタ
CEnemy::CEnemy(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.5, h);
	Texture(pt, TEXCOORD);
}

//更新処理
void CEnemy::Update()
{

}