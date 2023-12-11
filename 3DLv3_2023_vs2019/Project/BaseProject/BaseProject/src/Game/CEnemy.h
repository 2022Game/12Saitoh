#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
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

	// 描画処理
	void Render();

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose, // Tポーズ
		eIdle,	// アイドル

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
	};

	static const AnimData ANIM_DATA[];

	static CEnemy* spInstance;
	CModel* mModel;
};

#endif
