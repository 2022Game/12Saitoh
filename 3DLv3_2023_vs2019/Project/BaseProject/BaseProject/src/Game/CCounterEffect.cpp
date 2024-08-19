#include "CCounterEffect.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.07f
// �G�t�F�N�g�̃T�C�Y
#define EFFECTSIZE 8.0f

// �ʏ�U���G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CCounterEffect::msAnimData = TexAnimData(2, 5, false, 9, ANIM_TIME);

// �R���X�g���N�^
CCounterEffect::CCounterEffect(const CVector& hitpos)
	: CBillBoardImage(COUNTER_EFFECTDATA1, ETag::eEffect, ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// �f�X�g���N�^
CCounterEffect::~CCounterEffect()
{
}

// �`��ʒu���擾
CVector CCounterEffect::HitPos()const
{
	return mHitPos;
}

// �X�V����
void CCounterEffect::Update()
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