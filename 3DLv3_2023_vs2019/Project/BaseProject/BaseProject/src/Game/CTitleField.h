#pragma once
#include "CObjectBase.h"

class CModel;

class CTitleField : public CObjectBase
{
public:
	// �R���X�g���N�^
	CTitleField();
	// �ł��ƃ��N�^
	~CTitleField();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

private:
	CModel* mpModel;
};