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
class NavNode;
class CEnemy : public CCharacter3
{
private:
	//敵の状態
	enum class EState
	{
		EIDLE,		//待機
		ECHASE,		//追跡
		ELOST,		//見失った
		EATTACK		//攻撃
	};
	//敵の状態
	EState mState;

	CCollider mCollider1;	//頭コライダ
	CCollider mCollider2;	//胴体コライダ
	CCollider mCollider3;	//下半身コライダ
	NavNode* mpNextNode;
	NavNode* mpLostNode;

	static CEnemy* spInstance;
	static CModel sModel;	//モデルデータ
	static int sNum;		//敵の数

	int mHp;				//ヒットポイント
	int mBulletTime;		//射撃間隔(α版)
	int mIdleTime;			//待機中の動作を決める
	int mRandIdle;			//ランダム値を保存

	//雪玉を飛ばしているかの判定フラグ
	bool mFlag;

	//プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;
	//攻撃できる範囲内かの判定
	bool AttackRange() const;
	//指定の座標まで移動
	bool MoveTo(const CVector& target);

	//待機状態の更新処理
	void UpdateIdle();
	//追跡状態の更新処理
	void UpdateChase();
	//見失った状態の更新処理
	void UpdateLost();
	//攻撃状態の更新処理
	void UpdateAttack();

public:
	//コンストラクタ
	CEnemy();
	//CEnemy(位置,回転,拡縮)
	CEnemy(const CVector& position, const CVector& rotation,
		const CVector& scale);
	//デストラクタ
	~CEnemy();
	//インスタンスのポインタの取得
	static CEnemy* Instance();

	//敵の数を初期化
	int ResetNum();
	//敵の数を取得
	int Num()const;

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