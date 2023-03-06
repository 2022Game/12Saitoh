#pragma once
#include <vector>
#include "CRectangle.h"
#include "CTexture.h"
#include "CInput.h"
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"
#include "CCharacter.h"
#include "CTaskManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CUi.h"
#include "CBack.h"
class CApplication
{
private:
	CGame* mpGame;
	CPlayer* mpPlayer;  //�v���C���[�N���X�̃|�C���^
	static CUi* spUi;   //UI�N���X�̃|�C���^
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[
	static CCharacterManager mCharacterManager;
	static CTexture mTexture;
	static CTexture mTexture2;
	CInput mInput;
public:
	~CApplication();
	static CUi* Ui(); //UI�N���X�̃C���X�^���X���擾
	//static CTaskManager* TaskManager();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	static CTexture* Texture2();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};