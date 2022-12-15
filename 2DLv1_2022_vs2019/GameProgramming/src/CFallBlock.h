#pragma once
#include "CCharacter.h"
/*
CFallBlock
落下ブロッククラス
マップのブロックにしようする
*/
class CFallBlock : public CCharacter
{
public:
	//落下判定値を取得
	int Fall();
	//衝突処理2
	void Collision();
	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);
	//CFallblock(X座標、Y座標、幅、高さ、テクスチャのポインタ)
	CFallBlock(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update();
private:
	int sfall;
};