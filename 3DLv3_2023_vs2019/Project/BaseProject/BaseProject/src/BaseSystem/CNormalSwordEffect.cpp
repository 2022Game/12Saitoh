#include "CNormalSwordEffect.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.1f
// �G�t�F�N�g�̃T�C�Y
#define EFFECTSIZE 8.0f

// �ʏ�U���G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CNormalSwordEffect::msAnimData = TexAnimData(1, 5, false, 5, ANIM_TIME);

// �R���X�g���N�^
CNormalSwordEffect::CNormalSwordEffect(const CVector& hitpos, const CVector& offsetPos, const CMatrix& offsetRot)
	: CBillBoardImage(EFFECTDATA, ETag::eEffect , ETaskPauseType::eGame)
	, mHitPos(hitpos)
	, mOffsetPos(offsetPos)
	, mOffsetRot(offsetRot)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// �f�X�g���N�^
CNormalSwordEffect::~CNormalSwordEffect()
{
}

// �`��ʒu���擾
CVector CNormalSwordEffect::HitPos()const
{
	return mHitPos;
}

// �I�t�Z�b�g�ʒu��ݒ�
void CNormalSwordEffect::OffsetPos(const CVector& pos)
{
	mOffsetPos = pos;
}

// �I�t�Z�b�g��]�l��ݒ�
void CNormalSwordEffect::OffsetRot(const CMatrix& rot)
{
	mOffsetRot = rot;
}

// �X�V����
void CNormalSwordEffect::Update()
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