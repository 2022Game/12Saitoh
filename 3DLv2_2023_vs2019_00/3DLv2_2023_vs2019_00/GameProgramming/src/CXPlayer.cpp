#include "CXPlayer.h"

CXPlayer::CXPlayer()
	: mColSpherHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSpherBody(this, nullptr, CVector(), 0.5f)
	, mColSpherSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	mTag = ETag::EPLAYER;
}

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSpherHead.Matrix(&mpCombinedMatrix[12]);
	//��
	mColSpherBody.Matrix(&mpCombinedMatrix[9]);
	//��
	mColSpherSword.Matrix(&mpCombinedMatrix[22]);
}

void CXPlayer::Update()
{
	//�ړ�����
	//�U�����[�V�������͈ړ����]���ł��Ȃ��悤�ɂ���
	if (mInput.Key('W') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(1, true, 60);
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	//�ҋ@���[�V����
	else if (CXCharacter::AnimationIndex() != 3 && 
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}
	if (mInput.Key('A') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}
	if (mInput.Key('D') &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}
	//�U������
	if (mInput.Key(VK_SPACE) &&
		CXCharacter::AnimationIndex() != 3 &&
		CXCharacter::AnimationIndex() != 4)
	{
		CXCharacter::ChangeAnimation(3, false, 30);
	}
	if (CXCharacter::AnimationIndex() == 3 && 
		CXCharacter::IsAnimationFinished() == true)
	{
		CXCharacter::ChangeAnimation(4, false, 30);
	}
	//�U�����[�V�������I�������ҋ@���[�V�����ɂ���
	if (CXCharacter::AnimationIndex() == 4 &&
		CXCharacter::IsAnimationFinished() == true)
	{
		CXCharacter::ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}