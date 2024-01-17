#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderLine.h"
#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	// インスタンスを取得
	static CEnemy* Instance();
	// コンストラクタ
	CEnemy();

	// 更新処理
	void Update();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画処理
	void Render();

private:
	// 待機状態
	void Update_Idle();
	// 移動状態
	void Update_Move();
	// 攻撃状態
	void Update_Attack();
	// 死亡状態
	void Update_Death();

	// コライダーの更新処理
	void ColliderUpdate();

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer()const;

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// アイドル
		eIdle2,		// アイドル2
		eIdle3,		// アイドル3
		eWalk,		// 歩き
		eRun,		// 走り
		eAttack_Bite,// 噛みつき
		eAttack_Scratching,// 引っ掻き
		eAttack_Tail,// 尻尾攻撃
		eRoar,		// 咆哮
		eFear_Right,// 怯み(右)
		eFear_Left,	// 怯み(左)
		eDeath,		// 死亡
		eFly,		// 飛行
		eFlyStart,	// 飛行始め
		eFly_Attack,// 飛行中攻撃
		eFly_Breath,// 飛行中ブレス
		eFly_Fear,	// 飛行中怯み
		eFly_Death,	// 飛行中死亡

		Num
	};
	// アニメーションの切り替え
	void ChangeAnimation(EAnimType type);
	// アニメーションデータテーブル
	struct AnimData
	{
		std::string path;	// アニメーションデータパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float motionValue;	// モーション値
	};
	
	// 敵の状態
	enum class EState
	{
		None = -1,

		eIdle,		// 待機
		eMove,		// 移動
		eAttack,	// 攻撃
		eDeath,		// 死亡
	};
	EState mState;	// 敵の状態

	// アニメーションデータテーブル
	static const AnimData ANIM_DATA[];
	// インスタンス
	static CEnemy* spInstance;

	CModel* mModel;
	CColliderSphere* mpHeadCol;		// 頭
	CColliderSphere* mpBodyCol;		// 体
	CColliderSphere* mpTailCol;		// 尻尾
	CColliderLine* mpColliderLine;	// 地面との当たり判定用

	CVector mMoveSpeed;	// 移動速度
	bool mIsGrounded;	// 接地しているかどうか
	int mIdleTime;		// アイドル管理用の時間
	float mElapsedTime;	// 経過時間計測長
};
#endif
