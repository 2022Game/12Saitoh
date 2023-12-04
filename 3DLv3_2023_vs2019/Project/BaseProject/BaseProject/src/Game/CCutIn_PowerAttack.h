#pragma once
#include "CCutInCamera.h"

// 強闘技用カットイン
class CCutIn_PowerAttack : public CCutInCamera
{
public:
	// コンストラクタ
	CCutIn_PowerAttack();
	// デストラクタ
	~CCutIn_PowerAttack();

	// カットイン開始
	bool Start() override;
	// カットイン終了
	bool End() override;

	// カットインに必要な情報を設定
	void Setup(CObjectBase* obj);
	
	// 更新
	void Update() override;
private:
	// ステップ0 カメラを回転
	void CutInStep0();
	// ステップ1 カットイン終了時の待ち
	void CutInStep1();

	int mCutInStep;		// カットインのステップ
	float mElapsedTime;	// 経過時間計測長

	CVector mCenterPos;		// カットインの中心地点
	CVector mStartSideVec;	// 開始時のプレイヤーの横方向のベクトル
	float mStartAngleY;		// 開始のカメラのY軸角度
	float mStartAngleX;		// 開始のカメラのX軸角度
};