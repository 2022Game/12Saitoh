#pragma once
#include "CCharacter.h"
/*
* CBlock
* ブロッククラス
* マップのブロックに使用する
*/
class CNclimbBlock : public CCharacter
{
public:
	//CNclimbBlock(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CNclimbBlock(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};