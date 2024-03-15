#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"
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
	// 攻撃処理
	// UpdateBattleの中で行う
	void UpdateAttack();

	// アニメーションの切り替え
	void ChangeAnimation(EDragonAnimType type);

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;

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
	CColliderSphere* mpBodyCol;		//押し戻し用コライダー
	CColliderSphere* mpDamageCol;	// ダメージ用コライダー
	CColliderSphere* mpAttackMouthCol;// 噛みつき攻撃用コライダー

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか
	bool mIsAngry;		// 怒り状態かどうか

	int mAngryStandardValue;// 怒り値の基準値
	int mAngryValue;	// 怒り値

	float mAngryElapsedTime;// 怒り経過時間計測長
	float mElapsedTime;		// 経過時間計測長

};
#endif