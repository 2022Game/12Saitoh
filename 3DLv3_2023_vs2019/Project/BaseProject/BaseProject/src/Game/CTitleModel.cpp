#include "CTitleModel.h"
#include "CInput.h"
#include "CTitleSword.h"

#define DRAWN_FRAME 35.0f

// インスタンス
CTitleModel* CTitleModel::spInstance = nullptr;

// タイトル用モデルのアニメーションデータのテーブル
const CTitleModel::AnimData CTitleModel::ANIM_DATA[] =
{
	/*				　パス								　 ループ  フレーム	*/
	{ "",													true,	 0.0f,	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",					true,	 601.0f,},	// 待機(納刀)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	 601.0f,},	// 待機(抜刀)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	 101.0f,},	// 待機中抜刀動作
};

// コンストラクタ
CTitleModel::CTitleModel()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mUpdateStep(0)
	, mIsDrawn(false)
{
	// インスタンスを設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("TitleModel");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
	SetAnimationSpeed(0.5f);

	// タイトルモデル用の剣を生成
	mpTitleSword = new CTitleSword();
	mpTitleSword->Rotate(CVector(0.0f, 0.0f, 0.0f));
}

// デストラクタ
CTitleModel::~CTitleModel()
{
}

// インスタンスを取得
CTitleModel* CTitleModel::Instance()
{
	return spInstance;
}

// アニメーションを切り替え
void CTitleModel::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::eNone < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, 0.0f);
}

// 更新処理
void CTitleModel::Update()
{
	switch (mUpdateStep)
	{
	case 0:// 待機(納刀)中の処理
		// マウスクリック時に抜刀動作を行う
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
			SetAnimationSpeed(0.7f);
			mUpdateStep++;
		}
		break;
	case 1:// 抜刀中の処理
		// 抜刀中の剣の持ち替え処理
		if (DRAWN_FRAME <= GetAnimationFrame())
		{
			mIsDrawn = true;
			mUpdateStep++;
		}
		break;
	case 2:
		// 抜刀動作が終わったら待機(抜刀)に切り替える
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eIdle_Combat);
			SetAnimationSpeed(0.5f);
			mUpdateStep++;
		}
		break;
	case 3:// 待機(抜刀)中の処理
		break;
	}
	// キャラクターの更新
	CXCharacter::Update();
}

// 描画処理
void CTitleModel::Render()
{
	CXCharacter::Render();
}

// 抜刀中かどうか
bool CTitleModel::IsDrawn()
{
	return mIsDrawn;
}