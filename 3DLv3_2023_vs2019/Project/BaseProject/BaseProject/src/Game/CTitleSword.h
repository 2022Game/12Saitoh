#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CTitleModel.h"

class CTitleSword : public CWeapon
{
public:
	// �R���X�g���N�^
	CTitleSword();
	// �f�X�g���N�^
	~CTitleSword();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

	// �s��̎擾
	CMatrix Matrix() const override;
private:
	CModel* mpModel;
};