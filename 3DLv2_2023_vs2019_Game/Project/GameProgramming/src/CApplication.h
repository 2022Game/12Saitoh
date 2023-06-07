#pragma once
#include <vector>
#include "CTexture.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CSound.h"
#include "CVector.h"
#include "CModel.h"
#include "CMatrix.h"
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CMapManager.h"

class CApplication
{
private:
	static CUi* spUi; //UI�N���X�̃|�C���^
	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	//C5���f��
	CModel mModelC5;
	CPlayer mPlayer;
	CMatrix mMatrix;
	CModel mBackGround; //�w�i���f��
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	CMapManager* mpMapManager;
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CInput mInput;
	CFont mFont;
	bool CameraFlag = false; //�f�o�b�O�p

public:
	~CApplication();
	static CUi* Ui(); //UI�N���X�̃C���X�^���X���擾
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	//static CTaskManager* TaskManager();
	static CTexture* Texture();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};