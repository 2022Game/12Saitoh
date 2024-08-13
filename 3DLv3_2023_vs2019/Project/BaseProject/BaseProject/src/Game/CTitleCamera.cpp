#include "CTitleCamera.h"
#include "CInput.h"
#include "Maths.h"

// �J�����̉�]���x
#define ROTATE_SPEED 0.1f
// �J�����̏㉺��]�͈̔�
#define ROTATE_RANGE_X 70.0f

// �R���X�g���N�^
CTitleCamera::CTitleCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// �f�X�g���N�^
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

// �X�V
void CTitleCamera::Update()
{
	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);

	// �J�����̃x�[�X�̍X�V����
	//CCamera::Update();
}
