#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"
#include "CSword.h"
#include "PlayerData.h"
#include "CInput.h"
#include "CCamera.h"
#include "CCutIn_PowerAttack.h"
class CSPGauge;
class CHPGauge;
class CSword;

class CFlamethrower;
/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CXCharacter
{
public:
	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

	// 納刀状態か抜刀状態か判定
	bool IsDrawn();
	// 無敵状態かどうか
	bool IsInvincible()const;


	void TakeDamage(int damage) override;
private:
	// 待機状態
	void Update_Idle();
	// 移動状態
	void Update_Move();
	//ダッシュ移動状態
	void Update_FastMove();
	// 攻撃処理
	void Update_Attack();
	// 攻撃待ち処理
	void Update_AttackWait();
	// 攻撃終了
	void Update_AttackEnd();
	// プレイヤーの向いている方向のベクトルを取得
	void Update_AttackAngleVec(CVector* direction);
	// 通常攻撃1-1処理
	void Update_NormalAttack1();
	// 通常攻撃1-2処理
	void Update_NormalAttack2();
	// 通常攻撃1-3処理
	void Update_NormalAttack3();
	// ジャンプ攻撃処理
	void Update_AttackUp();
	// 空中攻撃1-1処理
	void Update_AirAttack1();
	// 空中攻撃1-2処理
	void Update_AirAttack2();
	// 空中攻撃1-3処理
	void Update_AirAttack3();
	// 空中攻撃1-4処理
	void Update_AirAttack4();
	// 闘技処理
	void Update_SpecialMove();
	// 弱闘技処理
	void Update_Weak_SpecalMove();
	// 強闘技処理
	void Update_Strong_SpecalMove();
	// 強闘技中のプレイヤーの移動処理
	void Strong_SpecalMove_Move();
	// 回避動作
	void Update_Avoidance();
	// 死亡処理
	void Update_Die();

	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 抜納状態を切り替える
	void SwitchDrawn();
	// 抜納の切り替え処理
	void Update_SwitchDrawn();

	// 闘技に必要な情報を設定
	// type : 再生するアニメーション　cutin : カットインの有無
	void SetSpecalMove(EAnimType type, bool cutin);

	// 無敵状態にする
	void InvincibleON();
	// 無敵状態を解除する
	void InvincibleOFF();

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// プレイヤーの状態
	enum class EState
	{
		None = -1,
		eIdle,		// 待機
		eMove,		// 移動
		eFastMove,	// ダッシュ移動
		eAvoidance,	// 回避
		eAttack,	// 攻撃
		eSpecalMove,// 闘技
		eDie,		// 死亡
	};
	EState mState;		// プレイヤーの状態
	// ステータスの切り替え
	void ChangeState(EState state);

	CVector mInput_save;// 入力ベクトルを仮保存
	CVector mMoveSpeed;	// 移動速度
	CVector mCamForward;// メインカメラの正面ベクトル
	CVector mCamSide;	// メインカメラのサイドベクトル

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsDrawn;		// 納刀状態か抜刀状態か判定 true:抜刀 false:納刀
	bool mIsAirAttack;	// 空中攻撃を行ったかどうか
	bool mIsCounter;	// カウンター構え中に攻撃が当たったかどうか
	bool mIsDash;		// ダッシュ中かどうか
	bool mIsAvoid;		// 回避中かどうか
	bool mSPZeroFlag;	// スタミナが0になった時のスタミナ回復中のフラグ
	bool mIsUpdateInput;// 入力情報を更新するフラグ
	bool mIsInvincible;// 無敵状態かどうか

	int mStateStep;		// 各状態の段階
	int mAttackStep;	// 攻撃の段階
	int mSPAttackStep;	// 強闘技の攻撃段階

	float mHPRecoveryTime;	// HP回復調整用

	CColliderLine* mpColliderLine;	// 接地判定用線分コライダ
	CColliderLine* mpColliderLine2;	// 壁との当たり判定用
	CColliderLine* mpColliderLine3;	// 壁との当たり判定用
	CColliderCapsule* mpBodyCol;	// 押し戻し用コライダー
	CColliderCapsule* mpDamageCol;	// ダメージ用コライダー

	CTransform* mpRideObject;
	CCutIn_PowerAttack* mpCutIn_PowerAttack;

	CHPGauge* mpHPGauge;	// HPゲージ
	CSPGauge* mpSPGauge;	// SPゲージ
	CSword* mpSword;		// 剣

	CFlamethrower* mpFlamethrower;
};