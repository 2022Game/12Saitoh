#ifndef CDRAGON_H
#define CDRAGON_H
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
private:
	// 待機状態
	void UpdateIdle();

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,
		
		eIdle1,			// 待機1
		eIdle2,			// 待機2
		eLand,			// 着地
		eRun,			// 走り
		eWalk,			// 歩き
		eScream,		// 咆哮
		eSleep,			// 寝る
		eTakeOff,		// 離陸
		eAttackFlame,	// ブレス攻撃
		eAttackHand,	// 飛び掛かり攻撃
		eAttackMouth,	// 噛みつき攻撃
		eDie,			// 死亡
		eFlyFlame,		// 空中ブレス攻撃
		eFlyForward,	// 空中前進
		eFlyGlide,		// 空中滑空
		eFlyIdle,		// 空中アイドル
		eGetHit,		// のけ反り

		Num
	};
	// アニメーションの切り替え
	void ChangeAnimation(EAnimType type);
	// アニメーションのデータテーブル
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

		eIdle,	// 待機
		eMove,	// 移動
		eAttack,// 攻撃
		eDeath,	// 死亡
	};
	EState mState;	// 敵の状態

	// アニメーションデータテーブル
	static const AnimData ANIM_DATA[];
	static CDragon* spInstance;
};
#endif