#pragma once
#include "CCharacter.h"
class CSky : public CCharacter
{
public:
	//CSky(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CSky(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};