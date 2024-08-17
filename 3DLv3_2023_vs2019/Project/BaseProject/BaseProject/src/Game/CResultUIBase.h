#pragma once
#include "CTask.h"

class CFont;
class CText;
class CLightingButton;
// リザルトシーンのUIベースクラス
class CResultUIBase : public CTask
{
public:
	// コンストラクタ
	CResultUIBase();
	// デストラクタ
	~CResultUIBase();

	// 更新処理
	virtual void Update();
	// 描画処理
	virtual void Render();

	// リザルト終了画面か
	bool IsEnd() const;
	// ゲームに再挑戦するか
	bool IsReTry() const;
	// タイトル画面に戻るか
	bool IsTitle() const;

	// UI処理開始
	virtual void Start();
	// UI処理終了
	virtual void End();
	// [リトライ]クリックのコールバック関数
	void OnClickReTry();
	// [タイトルへ戻る]クリック時のコールバック関数
	void OnClickTitle();

private:
	bool mIsEnd;		// リザルト画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

protected:
	CFont* mpResultFont;	// リザルトロゴのフォント
	CText* mpResultLogo;	// リザルトロゴ
	std::vector<CLightingButton*> mButton;
};