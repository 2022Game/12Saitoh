#pragma once
#include <vector>
#include "CRectangle.h"
#include "CCharacter.h"
#include "CCharacterManager.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CInput.h"
#include "CFont.h"
#include "CGame.h"
#include "CSound.h"
#include "CBackGround.h"
#include "CHeart.h"
#include "CGage.h"
#include "CFont2.h"
#include "CSound.h"

class CApplication
{
private:
	enum class EState
	{ 
		ESTART,  //�Q�[���J�n
		EPLAY,	 //�Q�[����
		ESTAGE1, //�X�e�[�W1
		ESTAGE2, //�X�e�[�W2
		EBOSS,   //�{�X�X�e�[�W
		ECLEAR,	 //�Q�[���N���A
		EOVER,	 //�Q�[���I�[�o�[
	};

	CGame* mpGame;
	EState mState;
	CInput mInput;
	CFont mFont;
	CSound mSoundBGM;     //�ʏ�BGM
	CSound mSoundBossBGM; //�{�XBGM
	CSound mSoundClear;   //�Q�[���N���A
	CSound mSoundOver;    //�Q�[���I�[�o�[
	CSound mSoundSE;      //SE

	static int sStageCount; //�X�e�[�W�ڍs���̃A�j���[�V�����J�E���^�@
	static bool sDelete;    //�e�N�X�`���������^�C�~���O���擾

	static CTexture mTexture;  //�v���C���[�e�N�X�`��
	static CTexture mTexture2; //�w�i�e�N�X�`��
	static CTexture mTexture3; //�X�^�[�g��ʃe�N�X�`��
	static CTexture mTexture4; //�Q�[���I�[�o�[��ʃe�N�X�`��
	static CTexture mTexture5; //�Q�[���N���A��ʃe�N�X�`��
	static CTexture mTexture6; //�w�i�e�N�X�`��
	static CTexture mTexture7; //�w�i�e�N�X�`��
	static CTexture mTexture8; //�w�i�e�N�X�`��
	static CTexture mTexture9; //�A�C�e���e�N�X�`��
	static CTexture mTexture100; //�n�[�g�\���e�N�X�`��
	static CTexture mTexture101; //�X�^�~�i�\���e�N�X�`��
	static CTexture mTexture102; //�X�^�~�i�Q�[�W�\���e�N�X�`��

public:
	static CTexture* Texture();
	static CTexture* Texture2();
	static CTexture* Texture3();
	static CTexture* Texture4();
	static CTexture* Texture5();
	static CTexture* Texture6();
	static CTexture* Texture7();
	static CTexture* Texture8();
	static CTexture* Texture9();
	static CTexture* Texture100();
	static CTexture* Texture101();
	static CTexture* Texture102();

	//sStageCount�̒l���擾
	static int StageCount();
	//mDelete�̔�����擾
	static bool Delete();

	//�R���X�g���N�^
	CApplication();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};