//プレイヤークラスのインクルード
#include "CPlayer.h"
#include"CApplication.h"
#include"CTaskManager.h"

#define TEXCOORD 0, 150, 238, 42 //テクスチャマッピング
//コンストラクタ
CPlayer::CPlayer(float x, float y, float w, float h, CTexture* pt)

{
	Set(x, y, w*1.3, h*2);
	Texture(pt, TEXCOORD);
	//インスタンスの設定
	spInstance = this;
}

//更新処理
void CPlayer::Update()
{
	if (mInput.Key('W'))
	{
		float y = Y() + 5.0f;
		Y(y);
	}
	if (mInput.Key('S'))
	{
		float y = Y() - 5.0f;
		Y(y);
	}
	if (mInput.Key('D'))
	{
		float x = X() + 5.0f;
		X(x);
	}
	if (mInput.Key('A'))
	{
		float x = X() - 5.0f;
		X(x);
	}
}

CPlayer* CPlayer::spInstance = nullptr;
CPlayer* CPlayer::Instance()
{
	return spInstance;
}