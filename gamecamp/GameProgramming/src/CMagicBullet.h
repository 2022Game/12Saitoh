#pragma once
#include "CCharacter.h"

class CMagicBullet : public CCharacter
{
private:
	int mDc;
public:
	//コンストラクタ
	CMagicBullet(float x, float y, float w, float h, CTexture* pt);
	//デストラクタ
	~CMagicBullet();

	//衝突処理
	void Collision(CCharacter* m, CCharacter* o);

	//更新処理
	void Update();

};