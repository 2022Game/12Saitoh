#include "CTargetCamera.h"

CTargetCamera* CTargetCamera::spTargetCamera = nullptr;

// コンストラクタ
CTargetCamera::CTargetCamera()
	: CCamera(CVector::zero,CVector::zero,false)
	, mIsPlaying(false)
	, mEyePos(CVector::zero)
	, mAtPos(CVector::zero)
{
}

// デストラクタ
CTargetCamera::~CTargetCamera()
{

}

// ターゲットカメラ開始
void CTargetCamera::Start()
{
	// 既にカメラがオンになっていたら開始しない
	if (IsPlaying()) return;

	// 再生中のカメラに自身を設定
	spTargetCamera = this;
	mIsPlaying = true;
	// 現在のカメラを自身に設定
	SetCurrent(true);
}

// ターゲットカメラ終了
void CTargetCamera::End()
{
	// 再生中で無ければ停止しない
	if (!IsPlaying()) return;

	// 再生中のカットインカメラを解除
	spTargetCamera = nullptr;
	mIsPlaying = false;

	// 現在のカメラからも解除
	SetCurrent(false);
}

void CTargetCamera::Update_Set(CObjectBase* Eyeobj, CObjectBase* Atobj)
{
	// ターゲットカメラに必要な情報を取得
	mEyePos = Eyeobj->Position();
	mAtPos = Atobj->Position();
}

bool CTargetCamera::IsPlaying() const
{
	return mIsPlaying;
}

void CTargetCamera::Update()
{
	// カメラを再生中で無ければ更新しない
	if (!IsPlaying()) return;
	// 視点
	CVector eye = mEyePos - mAtPos;
	eye.Y(0.0f);
	eye.Normalize();
	eye *= 40.0f;
	eye.Y(10.0f);
	mEye = mEyePos + eye;

	// 注視点
	// Y座標は基本的に0なので少し上げる
	mAt = mAtPos + CVector(0.0f, 35.0f, 0.0f);

	// 設定されているコライダーと衝突する場合は、
	// カメラの位置を押し出す
	ApplyCollision();

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);

}