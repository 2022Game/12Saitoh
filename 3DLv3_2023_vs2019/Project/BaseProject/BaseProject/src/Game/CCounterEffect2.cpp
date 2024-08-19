#include "CCounterEffect2.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.06f
// �G�t�F�N�g�̃T�C�Y
#define EFFECTSIZE 10.0f

// �ʏ�U���G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CCounterEffect2::msAnimData = TexAnimData(2, 5, false, 10, ANIM_TIME);

// �R���X�g���N�^
CCounterEffect2::CCounterEffect2(const CVector& hitpos)
	: CBillBoardImage(COUNTER_EFFECTDATA2, ETag::eEffect, ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// �f�X�g���N�^
CCounterEffect2::~CCounterEffect2()
{
}

// �`��ʒu���擾
CVector CCounterEffect2::HitPos()const
{
	return mHitPos;
}

// �X�V����
void CCounterEffect2::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	Scale(CVector::one * EFFECTSIZE);
	// �A�j���[�V�������I�������A�폜�t���O�𗧂Ă�
	if (IsEndAnim())
	{
		Kill();
	}
}