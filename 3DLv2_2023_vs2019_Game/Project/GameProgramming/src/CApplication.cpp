#include "CApplication.h"
#include <stdlib.h>
#include "glut.h"
#include "CVector.h"
#include "NavManager.h"
#include "CSceneManager.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"

CUi* CApplication::spUi = nullptr;


CApplication::~CApplication()
{
	delete spUi; //�C���X�^���XUi�̍폜
}

CUi* CApplication::Ui()
{
	return spUi; //�C���X�^���X�̃|�C���^��Ԃ�
}

CMatrix CApplication::mModelViewInverse;
const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture CApplication::mTexture;
CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//�Q�[���V�[���̓ǂݍ���
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	//�V�[���}�l�[�W���̍X�V
	CSceneManager::Instance()->Update();
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̏Փˏ���
	CTaskManager::Instance()->Collision();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	//�R���W�����}�l�[�W���`��
	CCollisionManager::Instance()->Render();
	//�f�o�b�O�p
	//�m�[�h�̕\��
	static bool sw = false;
	if (mInput.PushKey('V'))
	{
		sw = !sw;
	}
	if (sw)
	{
		NavManager::Instance()->Render();
	}
}