#pragma once
#include "CCutInCamera.h"

// ゲームオーバー用カットイン
class CCutIn_GameOver : public CCutInCamera
{
public:
	// コンストラクタ
	CCutIn_GameOver();
	// デストラクタ
	~CCutIn_GameOver();

	// カットイン開始
	bool Start() override;

	// カットインに必要な情報を設定
	void Setup(CObjectBase* obj);

	// 更新
	void Update() override;
private:
	// カメラを回転
	void CutIn();

	int mCutInStep;		// カットインのステップ
	float mElapsedTime;	// 経過時間計測長

	CVector mCenterPos;		// カットインの中心地点
	CVector mStartSideVec;	// 開始時のドラゴンの横方向のベクトル
	float mStartAngleY;		// 開始のカメラのY軸角度
	float mStartAngleX;		// 開始のカメラのX軸角度
};