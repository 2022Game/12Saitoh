#include "CResultUIBase.h"
#include "CLightingButton.h"
#include "CText.h"

// コンストラクタ
CResultUIBase::CResultUIBase()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault,false, false)
	, mIsEnd(false)
	, mSelectIndex(0)
{
	// リザルトロゴのフォントデータを生成
	mpResultFont = new CFont("res\\Font\\GoodChoice.ttf");
	mpResultFont->SetFontSize(120);
	mpResultFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpResultFont->SetLineLength(WINDOW_WIDTH);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpResultFont);
	SAFE_DELETE(mpResultLogo);

	int size = mButton.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButton[i];
		mButton[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButton.clear();
}

// 更新処理
void CResultUIBase::Update()
{
}

// 描画処理
void CResultUIBase::Render()
{
}

// リザルト画面終了か
bool CResultUIBase::IsEnd() const
{
	return mIsEnd;
}

// ゲームに再挑戦するか
bool CResultUIBase::IsReTry() const
{
	// 選択項目が1つ目ならば、再挑戦
	return mSelectIndex == 0;
}

// タイトル画面に戻るか
bool CResultUIBase::IsTitle() const
{
	// 選択項目が2つ目ならば、タイトル画面に戻る
	return mSelectIndex == 1;
}

// [リトライ]クリック時のコールバック関数
void CResultUIBase::OnClickReTry()
{
	if (mIsEnd) return;
	mSelectIndex = 0;
	mIsEnd = true;
}

// [タイトルへ戻る]クリック時のコールバック関数
void CResultUIBase::OnClickTitle()
{
	if (mIsEnd) return;
	mSelectIndex = 1;
	mIsEnd = true;
}

// UI処理開始
void CResultUIBase::Start()
{
}

// UI処理終了
void CResultUIBase::End()
{
}