#pragma once
#include "CTexture.h"
#include "CCharacter.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CCamera.h"
#include "CModelX.h"
#include "CInput.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CPaladin.h"
#include "CActionCamera.h"

class CApplication
{
private:
	static CMatrix mModelViewInverse;	//���f���r���[�̋t�s��
	static CTexture mTexture;
	static CUi* spUi;	//UI�N���X�̃|�C���^

	CInput mInput;
	CMatrix mMatrix;
	CModelX mModelX;
	CColliderMesh mColliderMesh; //���f������R���C�_�𐶐�
	CModel mModelC5;	         //C5���f��
	CModel mBackGround;          //�w�i���f��
	CModel mModel;
	CVector mEye;
	CSound mSoundBgm;	        //BGM
	CSound mSoundOver;	        //�Q�[���I�[�o�[
	CXCharacter mCharacter;		//�L�����N�^�N���X�̃C���X�^���X
	CXPlayer mXPlayer;			//�v���C���[�̃C���X�^���X
	CXEnemy mXEnemy;			//�G�̃C���X�^���X
	CModelX mKnight;			//�G(���m)�̃C���X�^���X
	CActionCamera mActionCamera;//�A�N�V�����J����
	CPaladin* mpPaladin;

	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
	CFont mFont;
public:
	~CApplication();
	static CUi* Ui();	//UI�N���X�̃C���X�^���X���擾
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();

	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};