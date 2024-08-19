#include "CSPMoveEffect_Sword.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.07f
// �G�t�F�N�g�̃T�C�Y
#define EFFECTSIZE 10.0f

// �ʏ�U���G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CSPMoveEffect_Sword::msAnimData = TexAnimData(3, 5, false, 14, ANIM_TIME);

// �R���X�g���N�^
CSPMoveEffect_Sword::CSPMoveEffect_Sword(const CVector& pos)
	: CBillBoardImage(SPMOVE_SWORD_EFFECTDATA, ETag::eEffect, ETaskPauseType::eGame)
	, mPos(pos)
{
	SetAnimData(&msAnimData);
	Position(mPos);
}

// �f�X�g���N�^
CSPMoveEffect_Sword::~CSPMoveEffect_Sword()
{
}

// �`��ʒu���擾
CVector CSPMoveEffect_Sword::HitPos()const
{
	return mPos;
}

// �X�V����
void CSPMoveEffect_Sword::Update()
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