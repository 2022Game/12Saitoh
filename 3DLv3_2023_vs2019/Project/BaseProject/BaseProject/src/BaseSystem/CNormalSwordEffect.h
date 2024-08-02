#pragma once
#include "CBillBoardImage.h"

#define EFFECTDATA "Effect/NormalAttack.png"

// �ʏ�U���G�t�F�N�g
class CNormalSwordEffect : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CNormalSwordEffect(const CVector& hitpos, const CVector& offsetPos, const CMatrix& offsetRot);
	// �f�X�g���N�^
	~CNormalSwordEffect();

	// �`��ʒu���擾
	CVector HitPos()const;
	// �I�t�Z�b�g���W��ݒ�
	void OffsetPos(const CVector& pos);
	// �I�t�Z�b�g��]�l��ݒ�
	void OffsetRot(const CMatrix& rot);

	// �X�V����
	void Update() override;

private:

	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mHitPos;	// �Փ˔��肪�N�����ʒu(�G�t�F�N�g�̕\�����W)
	CVector mOffsetPos;	// �I�t�Z�b�g�ʒu
	CMatrix mOffsetRot;	// �I�t�Z�b�g��]�l
};