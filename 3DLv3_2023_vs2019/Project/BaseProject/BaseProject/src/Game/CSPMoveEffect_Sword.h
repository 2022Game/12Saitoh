#pragma once
#include "CBillBoardImage.h"

#define SPMOVE_SWORD_EFFECTDATA "Effect/SPMoveEffect_Sword.png"

// �ʏ�U���G�t�F�N�g
class CSPMoveEffect_Sword : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CSPMoveEffect_Sword(const CVector& pos);
	// �f�X�g���N�^
	~CSPMoveEffect_Sword();

	// �`��ʒu���擾
	CVector HitPos()const;

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mPos;	// �G�t�F�N�g�̕\�����W
};