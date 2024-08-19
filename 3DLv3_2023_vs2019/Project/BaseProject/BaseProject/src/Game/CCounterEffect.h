#pragma once
#include "CBillBoardImage.h"

#define COUNTER_EFFECTDATA1 "Effect/Counter1.png"

// �ʏ�U���G�t�F�N�g
class CCounterEffect : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CCounterEffect(const CVector& hitpos);
	// �f�X�g���N�^
	~CCounterEffect();

	// �`��ʒu���擾
	CVector HitPos()const;

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mHitPos;	// �Փ˔��肪�N�����ʒu(�G�t�F�N�g�̕\�����W)
};