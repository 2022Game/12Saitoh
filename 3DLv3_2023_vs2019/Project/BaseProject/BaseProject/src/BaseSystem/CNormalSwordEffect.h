#pragma once
#include "CLineEffect.h"

// �ʏ�U���G�t�F�N�g
class CNormalSwordEffect : public CLineEffect
{
public:
	// �R���X�g���N�^
	CNormalSwordEffect(const CVector& start, const CVector& end);
	// �f�X�g���N�^
	~CNormalSwordEffect();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;
};