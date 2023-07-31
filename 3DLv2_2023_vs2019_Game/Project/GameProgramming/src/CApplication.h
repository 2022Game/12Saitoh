#pragma once
#include <vector>
#include "CTexture.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CUi.h"
#include "CInput.h"

class CApplication
{
private:
	//UI�N���X�̃|�C���^
	static CUi* spUi;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	static CTexture mTexture;

	CInput mInput;

public:
	~CApplication();
	//UI�N���X�̃C���X�^���X���擾
	static CUi* Ui();
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};