#pragma once
#include "CCharacter.h"
class CBlackBlock : public CCharacter
{
public:
	//CBlock(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlackBlock(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};