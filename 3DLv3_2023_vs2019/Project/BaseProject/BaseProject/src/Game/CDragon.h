#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"

class CFlamethrower;
class CSPFlamethrower;
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
	// 怯み中の処理
	void UpdateBattele_Fear();
	// 空中ブレス攻撃処理
	void UpdateSpecalAttack();

	// モーションブラー処理
	void UpdateMotionBlur();

	/* 通常攻撃各処理 */
	// 咆哮攻撃
	void Update_Sceream();
	// 噛みつき攻撃
	void Update_AttackMouth();
	// 飛び掛かり攻撃
	void Update_AttackHand();
	// 地上ブレス攻撃
	void Update_AttackFlame();
	// バックステップ
	void Update_BackStep();

	/* 空中ブレス攻撃各処理 */
	// 咆哮処理
	void UpdateSpAttack_Step0();
	// 離陸処理
	void UpdateSpAttack_Step1();
	// 空中移動処理1
	void UpdateSpAttack_Step2();
	// 空中移動処理2
	void UpdateSpAttack_Step3();
	// 空中アイドル及び高さ調整
	void UpdateSpAttack_Step4();
	// 空中ブレス攻撃
	void UpdateSpAttack_Step5();
	// 空中アイドル処理
	void UpdateSpAttack_Step6();
	//  空中移動処理3
	void UpdateSpAttack_Step7();
	// 着陸処理
	void UpdateSpAttack_Step8();
	// 目的地を取得
	CVector GetDestination() const;
	// 角度の取得
	float GetAngle() const;


	// 怒り状態の切り替え処理
	void ChangeAngry();
	// アニメーションの切り替え
	void ChangeAnimation(EDragonAnimType type);
	// ゲーム開始時にコライダーを生成
	void CreateCollider();
	// コライダーの更新処理
	void ColliderUpdate();

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;
	// バックステップできるかどうか
	bool IsBackStep() const;

	// 残りHPの割合を取得
	int GetHPPercent() const;
	// 戦闘アイドル時のランダム値生成用
	// 1〜3までの乱数を返す
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

		eIdle,		// 待機
		eBattle,	// 戦闘
		eSpAttack,	// 必殺技攻撃
		eDeath,		// 死亡
	};
	EState mState;	// 敵の状態

	// インスタンス
	static CDragon* spInstance;

	CColliderLine* mpColliderLine;	// 地面との当たり判定用
	CColliderLine* mpColliderLine2;	// 壁との当たり判定用
	CColliderLine* mpColliderLine3;	// 壁との当たり判定用

	//CColliderSphere* mpAttackMouthCol;// 噛みつき攻撃用コライダー

	////* ダメージ判定用コライダー *////
	/* 頭部 */
	CColliderCapsule* mpHeadDamageCol;	// 頭
	CColliderCapsule* mpNeckDamageCol;	// 首
	/* 胴体 */
	CColliderCapsule* mpBodyDamageCol;		// 胴体
	CColliderCapsule* mpTailDamageCol_Root;	// 尻尾(根本)
	CColliderCapsule* mpTailDamageCol_Tip;	// 尻尾(先端)
	/* 足(根本) */
	CColliderCapsule* mpLegDamageCol_RF_Root; // 右前足
	CColliderCapsule* mpLegDamageCol_LF_Root; // 左前足
	CColliderCapsule* mpLegDamageCol_RB_Root; // 右後ろ足
	CColliderCapsule* mpLegDamageCol_LB_Root; // 左後ろ足
	/* 足 */
	CColliderCapsule* mpLegDamageCol_RF;	// 右前足
	CColliderCapsule* mpLegDamageCol_LF;	// 左前足
	CColliderCapsule* mpLegDamageCol_RB;	// 右後ろ足
	CColliderCapsule* mpLegDamageCol_LB;	// 左後ろ足
	/* 足(足先) */
	CColliderSphere* mpLegDamageCol_RF_Tip; // 右前
	CColliderSphere* mpLegDamageCol_LF_Tip; // 左前
	CColliderSphere* mpLegDamageCol_RB_Tip; // 右後ろ足
	CColliderSphere* mpLegDamageCol_LB_Tip; // 左後ろ足
	/* 翼 */

	////* 押し戻し用コライダー *////
	/* 頭部 */
	CColliderCapsule* mpHeadCol;	// 頭
	CColliderCapsule* mpNeckCol;	// 首
	/* 胴体 */
	CColliderCapsule* mpBodyCol;	// 胴体
	CColliderCapsule* mpTailCol_Root;// 尻尾(根本)
	CColliderCapsule* mpTailCol_Tip;// 尻尾(先端)
	/* 足(根本) */
	CColliderCapsule* mpLegCol_RF_Root; // 右前足
	CColliderCapsule* mpLegCol_LF_Root;	// 左前足
	CColliderCapsule* mpLegCol_RB_Root;	// 右後ろ足
	CColliderCapsule* mpLegCol_LB_Root;	// 左後ろ足
	/* 足 */
	CColliderCapsule* mpLegCol_RF;	// 右前足
	CColliderCapsule* mpLegCol_LF;	// 左前足
	CColliderCapsule* mpLegCol_RB;	// 右後ろ足
	CColliderCapsule* mpLegCol_LB;	// 左後ろ足
	/* 足(足先) */
	CColliderSphere* mpLegCol_RF_Tip; // 右前
	CColliderSphere* mpLegCol_LF_Tip; // 左前
	CColliderSphere* mpLegCol_RB_Tip; // 右後ろ足
	CColliderSphere* mpLegCol_LB_Tip; // 左後ろ足

	////* 攻撃用のコライダー *////
	CColliderSphere* mpAttackMouthCol;	// 噛みつき攻撃


	CFlamethrower* mpFlamethrower; // 火炎放射(地上)
	CSPFlamethrower* mpSpFlamethrower; // 火炎放射(空中)
	CVector mMoveSpeed;			// 移動速度
	CVector mSaveDestination;	// 目的地の保存用
	CVector mSaveVec;			// ベクトルの保存用

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsAngry;		// 怒り状態かどうか
	bool mChangeAngry;	// 怒り状態に移行中かどうか

	int mAngryStandardValue;// 怒り値の基準値
	int mAngryValue;	// 怒り値
	int mFearValue;		// 怯み値
	int mRandSave;		// 乱数保存用
	int mBatteleStep;	// 戦闘処理の段階
	int mAttackStep;	// 攻撃処理の段階
	int mFearStep;		// 怯み処理の段階
	int mSpAttackStep;	// 必殺技攻撃の攻撃段階
	int mSpAttackNum;	// 必殺技攻撃を行った回数を記憶

	float mElapsedTime;		// 経過時間計測長
	float mAngryElapsedTime;// 怒り経過時間計測長
	float mChaseElapsedTime;// 移動経過時間計測長
	float mFearElapsedTime; // 怯み中の経過時間計測長
	float mAngle;	// ステージ中心から見たドラゴンの角度
	float mMotionBlurRemainTime; // モーションブラーを掛ける残り時間

	// デバッグ用
	int mDamage;
};
#endif