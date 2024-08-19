#pragma once
#pragma once
#include "CBillBoardImage.h"

#define COUNTER_EFFECTDATA2 "Effect/Counter2.png"

// �ʏ�U���G�t�F�N�g
class CCounterEffect2 : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CCounterEffect2(const CVector& hitpos);
	// �f�X�g���N�^
	~CCounterEffect2();

	// �`��ʒu���擾
	CVector HitPos()const;

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mHitPos;	// �Փ˔��肪�N�����ʒu(�G�t�F�N�g�̕\�����W)
};