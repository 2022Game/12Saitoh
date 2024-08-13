#pragma once
#include "CXCharacter.h"
#include "CModel.h"

class CTitleSword;
class CTitleModel : public CXCharacter
{
public:
	// インスタンスのポインタの取得
	static CTitleModel* Instance();

	// コンストラクタ
	CTitleModel();
	// デストラクタ
	~CTitleModel();

	// 更新処理
	void Update();
	// 描画処理
	void Render();

	// 抜刀中かどうか
	bool IsDrawn();
private:
	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// アニメーションの種類
	enum class EAnimType 
	{
		eNone = -1,
		
		eTPose,				// Tポーズ
		eIdle,				// 待機
		eIdle_Combat,		// 待機(抜刀)
		eIdle_Drawn_Combat,	// 待機中抜刀動作

		Num
	};
	void ChangeAnimation(EAnimType type);

	// インスタンス
	static CTitleModel* spInstance;

	// タイトルモデル用の剣
	CTitleSword* mpTitleSword;
	// 更新処理の段階
	int mUpdateStep;
	// 抜刀中かどうか
	bool mIsDrawn;
};