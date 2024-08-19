#include "CSPMoveEndEffect.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.06f
// �G�t�F�N�g�̃T�C�Y
#define EFFECTSIZE 15.0f

// �ʏ�U���G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CSPMoveEndEffect::msAnimData = TexAnimData(4, 5, false, 19, ANIM_TIME);

// �R���X�g���N�^
CSPMoveEndEffect::CSPMoveEndEffect(const CVector& pos)
	: CBillBoardImage(SPMOVE_END_EFFECTDATA, ETag::eEffect, ETaskPauseType::eGame)
	, mPos(pos)
{
	SetAnimData(&msAnimData);
	Position(mPos);
}

// �f�X�g���N�^
CSPMoveEndEffect::~CSPMoveEndEffect()
{
}

// �`��ʒu���擾
CVector CSPMoveEndEffect::HitPos()const
{
	return mPos;
}

// �X�V����
void CSPMoveEndEffect::Update()
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