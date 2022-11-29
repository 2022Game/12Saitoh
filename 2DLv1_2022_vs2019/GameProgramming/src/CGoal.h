#pragma once
#include "CCharacter.h"
/*
* CGoal
* ゴールクラス
* マップのゴールに使用する
*/
class CGoal : public CCharacter
{
public:
	//CGoal(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CGoal(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};