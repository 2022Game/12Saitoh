#pragma once
#include "CModel.h"
#include "CObjectBase.h"
#include "CPlayer.h"

class CSword :public CObjectBase
{
public:
	// �R���X�g���N�^
	CSword(const CVector& pos, const CVector& scale);
	// �f�X�g���N�^
	~CSword();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

private:
	CModel* mpModel;
	CPlayer* mpPlayer;
};