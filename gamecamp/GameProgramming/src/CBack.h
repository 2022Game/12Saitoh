#ifndef CBACK_H
#define CBACK_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
/*
背景クラス
キャラクタクラスを継承
*/
class CBack : public CCharacter
{
public:
	//コンストラクタ
	//CBack(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBack(float x, float y, float w, float h, CTexture* pt);
};
#endif