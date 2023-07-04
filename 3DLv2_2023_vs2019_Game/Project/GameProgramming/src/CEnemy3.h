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

	static float mDotX;		//横方向の角度
	static CModel sModel;	//モデルデータ

	//プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;
	bool mFlag;				//雪玉を飛ばしているかの判定フラグ

	//敵の状態
	enum class EState
	{
		EIDLE,		//待機
		ECHASE,		//プレイヤーを追跡
		EATTACK		//プレイヤーを攻撃
	};
	//敵の状態
	EState mState;
	//待機状態の更新処理
	void UpdateIdle();
	//追跡状態の更新処理
	void UpdateChase();
	//攻撃状態の更新処理
	void UpdateAttack();

public:
	//コンストラクタ
	CEnemy3();
	//CEnemy3(位置,回転,拡縮)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//更新処理
	void Update();
	//描画処理
	void Render();

	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);

};
#endif