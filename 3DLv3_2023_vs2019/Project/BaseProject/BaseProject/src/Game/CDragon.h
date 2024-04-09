#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"

class CFlamethrower;
/*
ドラゴンクラス
キャラクタクラスを継承
*/
class CDragon : public CXCharacter
{
public:
	// インスタンスを取得
	static CDragon* Instance();
	// コンストラクタ
	CDragon();
	// デストラクタ
	~CDragon();

	// 更新処理
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画処理
	void Render();

	// ダメージ処理
	void TakeDamage(int damage)override;
	/// <summary>
	/// 攻撃開始
	/// </summary>
	void AttackStart() override;
	/// <summary>
	/// 攻撃終了
	/// </summary>
	void AttackEnd() override;

private:
	// 待機状態
	void UpdateIdle();
	// 戦闘状態
	void UpdateBattle();
	// 戦闘中のアイドル処理
	void UpdateBattele_Idle();
	// 戦闘中の追跡処理
	void UpdateBattle_Chase();
	// 攻撃処理
	void UpdateAttack();

	// アニメーションの切り替え
	void ChangeAnimation(EDragonAnimType type);

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;

	// 残りHPの割合を取得
	int GetHPPercent() const;
	// 戦闘アイドル時のランダム値生成用
	// 1～3までの乱数を返す
	int BattleIdleRand() const;

	// プレイヤーとの距離
	enum class EDistanceType
	{
		None = -1,

		eNear,	// 近距離
		eMedium,// 中距離
		eFar,	// 遠距離
	};
	EDistanceType mDistanceType; // プレイヤーとの距離
	EDistanceType PlayerFromDistance(); // プレイヤーとの距離を取得

	// 敵の状態
	enum class EState
	{
		None = -1,

		eIdle,	// 待機
		eBattle,// 戦闘
		eDeath,	// 死亡
	};
	EState mState;	// 敵の状態

	// インスタンス
	static CDragon* spInstance;

	CColliderLine* mpColliderLine;	// 地面との当たり判定用
	CColliderLine* mpColliderLine2;	// 壁との当たり判定用
	CColliderLine* mpColliderLine3;	// 壁との当たり判定用
	CColliderSphere* mpBodyCol;		//押し戻し用コライダー
	CColliderSphere* mpDamageCol;	// ダメージ用コライダー
	CColliderSphere* mpAttackMouthCol;// 噛みつき攻撃用コライダー

	CFlamethrower* mpFlamethrower; // 火炎放射
	CVector mMoveSpeed;	// 移動速度

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsAngry;		// 怒り状態かどうか

	int mAngryStandardValue;// 怒り値の基準値
	int mAngryValue;	// 怒り値
	int mRandSave;		// 乱数保存用
	int mBatteleStep;	// 戦闘の段階

	float mElapsedTime;		// 経過時間計測長
	float mAngryElapsedTime;// 怒り経過時間計測長
	float mChaseElapsedTime;// 移動経過時間計測長
};
#endif