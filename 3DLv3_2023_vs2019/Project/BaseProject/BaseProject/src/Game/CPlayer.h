#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CRideableObject.h"
#include "CSword.h"
#include "PlayerData.h"
#include "CInput.h"
#include "CCamera.h"

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

	// 待機状態
	void Update_Idle();
	// 移動状態
	void Update_Move();
	//ダッシュ移動状態
	void Update_FastMove();
	// 攻撃
	void Update_Attack();
	// 攻撃待ち処理
	void Update_AttackWait();
	// 攻撃終了
	void Update_AttackEnd();
	// プレイヤーの向いている方向のベクトルを取得
	void Update_AttackAngleVec(CVector *direction);
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
	// 回避動作
	void Update_Avoidance();

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
private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,					// Tポーズ
		eIdle,					// 待機(納刀)
		eIdle_Combat,			// 待機(抜刀)
		eIdleAir,				// 空中待機(納刀)
		eIdleAir_Combat,		// 空中待機(抜刀)
		eIdle_Drawn_Combat,		// 待機中抜刀動作
		eIdle_Sheathed_Combat,	// 待機中納刀動作
		eRunStart,				// 走り開始(納刀)
		eRun,					// 走り(納刀)
		eRunEnd,				// 走り終了(納刀)
		eRunStart_Combat,		// 走り開始(抜刀)
		eRun_Combat,			// 走り(抜刀)
		eRunEnd_Combat,			// 走り終了(抜刀)
		eRun_Drawn_Combat,		// 走り中抜刀動作
		eRun_Sheathed_Combat,	// 走り中納刀動作
		eFastRunStart,			// ダッシュ開始
		eFastRun,				// ダッシュ
		eFastRunEnd,			// ダッシュ終了
		eRollStart,				// 回避動作開始(納刀)
		eRollEnd_idle,			// 回避後アイドル移行(納刀)
		eRollEnd_run,			// 回避後走り移行(納刀)
		eRollStart_Combat,		// 回避動作開始(抜刀)
		eRollEnd_idle_Combat,	// 回避後アイドル移行(抜刀)
		eRollEnd_run_Combat,	// 回避後走り移行(抜刀)
		eRunAttack,				// 走り後攻撃
		eNormalAttack1_1,		// 通常攻撃1-1
		eNormalWait1_1,			// 通常攻撃1-1攻撃待ち
		eNormalEnd1_1,			// 通常攻撃1-1終了
		eNormalAttack1_2,		// 通常攻撃1-2
		eNormalWait1_2,			// 通常攻撃1-2攻撃待ち
		eNormalEnd1_2,			// 通常攻撃1-2終了
		eNormalAttack1_3,		// 通常攻撃1-3
		eNormalWait1_3,			// 通常攻撃1-3攻撃待ち
		eNormalEnd1_3,			// 通常攻撃1-3終了
		eAirAttack1_1,			// 空中攻撃1-1
		eAirAttackWait1_1,		// 空中攻撃1-1攻撃待ち
		eAirAttackEnd1_1,		// 空中攻撃1-1終了
		eAirAttack1_2,			// 空中攻撃1-2
		eAirAttackWait1_2,		// 空中攻撃1-2攻撃待ち
		eAirAttackEnd1_2,		// 空中攻撃1-2終了
		eAirAttack1_3,			// 空中攻撃1-3
		eAirAttackWait1_3,		// 空中攻撃1-3攻撃待ち
		eAirAttackEnd1_3,		// 空中攻撃1-3終了
		eAirAttack1_4,			// 空中攻撃1-4
		eAirAttackEnd1_4,		// 空中攻撃1-4終了
		eAttack_Up,				// ジャンプ攻撃
		eLanding,				// 着地動作(納刀)
		eLandin_Combat,			// 着地動作(抜刀)
		eJumpStart,				// ジャンプ開始
		eJump,					// ジャンプ中
		eJumpEnd,				// ジャンプ終了
		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// 抜納状態を切り替える
	void SwitchDrawn();
	// 抜納の切り替え処理
	void Update_SwitchDrawn();

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// プレイヤーの状態
	enum class EState
	{
		None = -1,	//初期値

		eIdle,		// 待機
		eMove,		// 移動
		eFastMove,	// ダッシュ移動
		eAvoidance,	// 回避
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eAttackEnd,	// 攻撃終了
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;		// プレイヤーの状態

	CVector mInput_save;// 入力ベクトルを仮保存
	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか
	bool mIsDrawn;		// 納刀状態か抜刀状態か判定 true:抜刀 false:納刀
	bool mIsAirAttack;	// 空中攻撃を行ったかどうか
	
	int mAttackStep;	// 攻撃の段階 0:攻撃　1:攻撃待ち 2:攻撃終了
	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
	CSword* mpSword;
};
