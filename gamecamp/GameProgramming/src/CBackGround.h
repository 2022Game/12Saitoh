#pragma once
#include "CCharacter.h"
#include "CCharacterManager.h"

class CBackGround : public CCharacter
{
private:
	/*
	背景が移動中か判定
	true:移動中
	false:停止中
	*/
	static bool mMoveStage;
public:
	//コンストラクタ
	CBackGround::CBackGround(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);
	//更新処理
	void Update();
	//背景の状態を取得
	static bool GetMoveStage();
};