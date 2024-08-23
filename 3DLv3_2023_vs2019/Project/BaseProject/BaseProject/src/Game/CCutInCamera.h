#pragma once
#include "CCamera.h"

// カットインの時のカメラベースクラス
class CCutInCamera : public CCamera
{
public:
	// コンストラクタ
	CCutInCamera();
	//デストラクタ
	virtual ~CCutInCamera();

	// カットイン開始
	virtual bool Start();
	// カットイン開始(ポーズ無し)
	virtual bool Start_NoPause();
	// カットイン終了
	virtual bool End();

	// カットイン再生中かどうか
	bool IsPlaying() const;

	// 更新
	void Update() override;

protected:
	// 現在再生中のカットインカメラ
	static CCutInCamera* spCutInCamera;
	bool mIsPlaying;	// カットイン再生中かどうか
};