#ifndef CENEMY_H
#define CENEMY_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter
{
public:
	//コンストラクタ
	//CEnemy(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CEnemy(float x, float y, float w, float h, CTexture* pt);
	void Update();
};
#endif