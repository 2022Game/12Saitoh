#include "CResultScene.h"
#include "CTaskManager.h"
#include "CInput.h"
#include "CGameOverUI.h"

// コンストラクタ
CResultScene::CResultScene()
	: CTask(ETaskPriority::eMenu, 0, ETaskPauseType::eNone)
	, mIsPlay(false)
	, mResultStep(0)
	, mElapsedTime(0.0f)
{
	// リザルト画面の背景用の白イメージを読み込み
	mpBackImage = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eResult,
		true, false
	);
	// 黒背景に変更する
	// 最初は非表示のままなのでアルファ値も0
	mpBackImage->SetColor(0.0f, 0.0f, 0.0f, 0.0f);	
	// 背景イメージは画面全体に表示
	mpBackImage->SetPos(0.0f, 0.0f);
	mpBackImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CResultScene::~CResultScene()
{
	SAFE_DELETE(mpBackImage);
	// ポーズを解除
	CTaskManager::Instance()->UnPause(PAUSE_GAME);
}

// 更新処理
void CResultScene::Update()
{
	//背景の表示＆非表示に掛ける時間（秒）
	static const float Time = 0.25f;
	//背景の最大アルファ値
	static const float maxAlpha = 0.75f;

	// リザルトシーンの処理段階
	switch (mResultStep)
	{
	case 0: // 背景を表示
		// 背景の表示時間が経過していない
		if (mElapsedTime < Time)
		{
			// 経過時間に合わせてアルファ値を設定
			float alpha = mElapsedTime / Time;
			mpBackImage->SetAlpha(maxAlpha * alpha);
			// 1フレームの経過時間を加算
			mElapsedTime += Time::DeltaTime();
		}
		// 表示時間が経過
		else
		{
			// 背景を完全に表示して、次のステップへ
			mpBackImage->SetAlpha(maxAlpha);
			mElapsedTime = 0.0f;
			mResultStep++;
		}
		break;
	case 1: // リザルト画面中は何もしない
		break;
	case 2: // リザルト画面終了処理
		if (mElapsedTime < Time)
		{
			float alpha = mElapsedTime / Time;
			// 非表示にしていく
			mpBackImage->SetAlpha(Time * (1.0f - alpha));
			mElapsedTime += Time::DeltaTime();
		}
		else
		{
			SetEnable(false);
			SetShow(false);

			// アルファ値を0で非表示
			mpBackImage->SetAlpha(0.0f);
			mResultStep = 0;
			mElapsedTime = 0.0f;
			// 再生中のフラグを下ろす
			mIsPlay = false;
			// カーソル表示を無効にする
			CInput::ShowCursor(false);
			// ポーズを解除
			CTaskManager::Instance()->UnPause(PAUSE_GAME);
			// ゲームカメラを再開
			CCamera::MainCamera()->SetEnable(true);
		}
		break;
	}
}

// 描画処理
void CResultScene::Render()
{
	// 背景画像を読み込んでいたら描画
	if (mpBackImage != nullptr)
	{
		mpBackImage->Render();
	}
}

// 開始処理
void CResultScene::Start()
{
	SetEnable(true);
	SetShow(true);
	// 再生中のフラグを立てる
	mIsPlay = true;
	// ポーズを設定
	CTaskManager::Instance()->Pause(PAUSE_GAME);
	// カーソル表示を有効にする
	CInput::ShowCursor(true);
	// ゲームカメラも停止
	CCamera::MainCamera()->SetEnable(false);
}

// 終了処理
void CResultScene::End()
{
	// リザルト画面の処理段階を終了処理へ進める
	mResultStep++;
}

// リザルトシーン再生中かどうか取得
bool CResultScene::IsPlayResult()
{
	return mIsPlay;
}