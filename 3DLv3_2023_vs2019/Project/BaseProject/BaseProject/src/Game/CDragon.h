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
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,
		
		eIdle1,		// 待機
		
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