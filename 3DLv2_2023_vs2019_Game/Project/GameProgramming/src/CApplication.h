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
	//UI�N���X�̃|�C���^
	static CUi* spUi;
	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;
	static CTexture mTexture;

	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	CPlayer mPlayer;	//�v���C���[
	CMatrix mMatrix;
	CModel mBackGround; //�w�i���f��
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	CInput mInput;
	CFont mFont;
	CMapManager* mpMapManager;

	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
	static int mCameraFlag; //�f�o�b�O�p
	//�J�����̐ݒ�(��ŃJ�����N���X�ɂ܂Ƃ߂�)
	void Camera();
	//�e���f����ǂݍ���
	void CreateModel();
public:
	~CApplication();
	//UI�N���X�̃C���X�^���X���擾
	static CUi* Ui();
	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	static CTexture* Texture();
	//�f�o�b�O�p
	static int CameraFlag();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};