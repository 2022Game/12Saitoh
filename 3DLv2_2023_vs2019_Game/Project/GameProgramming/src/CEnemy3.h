#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy3 : public CCharacter3
{
private:
	CCollider mCollider1;	//頭コライダ
	CCollider mCollider2;	//胴体コライダ
	CCollider mCollider3;	//下半身コライダ

	int mHp;				//ヒットポイント
	int mBulletTime;		//射撃間隔(α版)

	static float mDistance;	//プレイヤーまでの距離
	static float mDotX;		//横方向の角度
	static CModel sModel;	//モデルデータ

	//プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;
	bool mFlag;				//雪玉を飛ばしているかの判定フラグ

public:
	//コンストラクタ
	CEnemy3();
	//CEnemy3(位置,回転,拡縮)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//更新処理
	void Update();

	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
};
#endif