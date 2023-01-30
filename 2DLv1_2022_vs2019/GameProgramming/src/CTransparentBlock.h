#pragma once
#include "CCharacter.h"
#include"CPlayer2.h"
/*
透明ブロッククラス
*/

class CTransparentBlock : public CCharacter
{
public:
	//衝突処理2
	void Collision();
	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);
	//CTrap(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CTransparentBlock(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update() {};
};