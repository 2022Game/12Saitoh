#pragma once
#include "CBillBoardImage.h"

#define SPMOVE_END_EFFECTDATA "Effect/SPMoveEndEffect1.png"

// �ʏ�U���G�t�F�N�g
class CSPMoveEndEffect : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CSPMoveEndEffect(const CVector& pos);
	// �f�X�g���N�^
	~CSPMoveEndEffect();

	// �`��ʒu���擾
	CVector HitPos()const;

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mPos;	// �G�t�F�N�g�̕\�����W
};