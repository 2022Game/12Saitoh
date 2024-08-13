#include "CTitleCamera.h"
#include "CInput.h"
#include "Maths.h"

// カメラの回転速度
#define ROTATE_SPEED 0.1f
// カメラの上下回転の範囲
#define ROTATE_RANGE_X 70.0f

// コンストラクタ
CTitleCamera::CTitleCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// デストラクタ
CTitleCamera::~CTitleCamera()
{
}

void CTitleCamera::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CTitleCamera::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

// 更新
void CTitleCamera::Update()
{
	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);

	// カメラのベースの更新処理
	//CCamera::Update();
}
