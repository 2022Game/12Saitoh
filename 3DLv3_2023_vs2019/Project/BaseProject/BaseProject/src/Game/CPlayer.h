#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CRideableObject.h"

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
	// 攻撃終了待ち
	void Update_AttackWait();
	// ジャンプ開始
	void Update_JumpStart();
	// ジャンプ中
	void Update_Jump();
	// ジャンプ終了
	void Update_JumpEnd();
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

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,			// Tポーズ
		eIdle,			// 待機
		eRunStart,		// 走り開始
		eRun,			// 走り
		eRunEnd,		// 走り終終了
		eFastRunStart,	// ダッシュ開始
		eFastRun,		// ダッシュ
		eFastRunEnd,	// ダッシュ終了
		eRoll,			// 回避動作
		eAttack,		// 攻撃
		eJumpStart,		// ジャンプ開始
		eJump,			// ジャンプ中
		eJumpEnd,		// ジャンプ終了

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		None = -1,	//初期値

		eIdle,		// 待機
		eMove,		// 移動
		eFastMove,	// ダッシュ移動
		eAttack,	// 攻撃
		eAttackWait,// 攻撃終了待ち
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
	};
	EState mState;		// プレイヤーの状態
	EState mState_save;	// プレイヤーの状態を一時的に保存

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか

	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
};
