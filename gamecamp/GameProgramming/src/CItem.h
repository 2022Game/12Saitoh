#pragma once
#include "CCharacter.h"

class CItem : public CCharacter
{
private:
	static int mIh;
	static int sNum; //アイテム数
public:
	//コンストラクタ
	CItem(float x, float y, float w, float h, CTexture* pt);
	static int Ih();
	//アイテム数を取得
	static int Num();

	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);

	//更新処理
	void Update();
};