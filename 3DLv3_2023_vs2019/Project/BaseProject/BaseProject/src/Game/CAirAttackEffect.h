#pragma once
#include "CBillBoardImage.h"

#define AIR_EFFECTDATA "Effect/AirAttack.png"

// �ʏ�U���G�t�F�N�g
class CAirAttackEffect : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CAirAttackEffect(const CVector& hitpos);
	// �f�X�g���N�^
	~CAirAttackEffect();

	// �`��ʒu���擾
	CVector HitPos()const;

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mHitPos;	// �Փ˔��肪�N�����ʒu(�G�t�F�N�g�̕\�����W)
};