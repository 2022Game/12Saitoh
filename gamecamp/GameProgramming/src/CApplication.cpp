#include "CApplication.h"
#include "CRectangle.h"

#define BGM "res\\BGM.wav"
#define BOSSBGM "res\\BossBGM.wav"
#define CLEAR "res\\Clear.wav"
#define OVER "res\\Over.wav"
#define SE "res\\Sytem_SE.wav"

bool CApplication::sDelete = false;
int CApplication::sStageCount = 0;
CTexture CApplication::mTexture;

CApplication::CApplication()
	: mpGame(nullptr)
	, mState()
	, mInput()
	, mFont()
	, mSoundBGM()
	, mSoundBossBGM()
	, mSoundClear()
	, mSoundOver()
	, mSoundSE()
{
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

bool CApplication::Delete()
{
	return sDelete;
}

int CApplication::StageCount()
{
	return sStageCount;
}

void CApplication::Start()
{
	//�e�N�X�`��
	mTexture.Load("PlayerImage.png");
	mTexture2.Load("�w�i(��) .png");
	mTexture3.Load("�^�C�g����� .png");
	mTexture4.Load("�Q�[���I�[�o�[.png");
	mTexture5.Load("�Q�[���N���A.png");
	mTexture6.Load("�w�i.png");
	mTexture7.Load("�w�i2.png");
	mTexture8.Load("�w�i3.png");
	mTexture9.Load("�A�C�e��.png");
	mTexture100.Load("�v���C���[HP.png");
	mTexture101.Load("MP�Q�[�W.png");
	mTexture102.Load("�v���C���[MP.png");
	//�t�H���g
	mFont.Load("FontWhite.png", 1, 64);
	//�T�E���h
	mSoundBGM.Load(BGM);
	mSoundBossBGM.Load(BOSSBGM);
	mSoundClear.Load(CLEAR);
	mSoundOver.Load(OVER);
	mSoundSE.Load(SE);

	mState = EState::ESTART;
	mpGame = new CGame();
}

void CApplication::Update()
{
	switch (mState)
	{
	case EState::ESTART:	//��Ԃ��X�^�[�g
		mpGame->Start();	//�X�^�[�g��ʕ\��
		if (mInput.Key(VK_RETURN))
		{
			mSoundSE.Play();
			//��Ԃ��v���C���ɂ���
			mState = EState::ESTAGE1;
			sDelete = false;
			sStageCount = 0;
			mSoundBGM.Repeat();
		}
		break;
	case EState::ESTAGE1:
		sDelete = false;
		sStageCount = 0;
		mpGame->Stage1();
		mState = EState::EPLAY;
	break;
	case EState::ESTAGE2:
		mpGame->Stage2();
		mState = EState::EPLAY;
		break;
	case EState::EBOSS:
		mpGame->Boss();
		mState = EState::EPLAY;
		sStageCount = 432;
		mSoundBossBGM.Repeat();
		break;
	case EState::EPLAY:
		mpGame->Update();
		if (CGame::Num() == 0)
		{
			sStageCount++;
		}
		if (sStageCount == 200)
		{
			mState = EState::ESTAGE2;
			sStageCount = 201;
		}
		else if (sStageCount == 430)
		{
			mSoundBGM.Stop();
			mState = EState::EBOSS;
			sStageCount = 431;
		}
		if (sStageCount >= 470)
		{
			//�{�X�̎��S�t���O����������
			if (CBoss::Instance4()->Death() == true)
			{
				mSoundBGM.Stop();
				mSoundBossBGM.Stop();
				mSoundClear.Play();
				mState = EState::ECLEAR;
				sDelete = true;
			}
		}
		//�v���C���[�����S������Q�[���I�[�o�[�����Ɉڍs
		if (CPlayer::Instance()->BoolDeath() == true)
		{
			mSoundBGM.Stop();
			mSoundBossBGM.Stop();
			mSoundOver.Play();
			mState = EState::EOVER;
			sDelete = true;
		}
		break;

	case EState::EOVER:
		//�Q�[���I�[�o�[����
		mpGame->Over();
		//�G���^�[�L�[���͎�
		if (mInput.Key('N'))
		{	
			mSoundSE.Play();
			mSoundOver.Stop();
			//�Q�[���̃C���X�^���X�폜
			delete mpGame;
			mpGame = nullptr;
			//�Q�[���̃C���X�^���X����
			mpGame = new CGame();
			//��Ԃ��X�^�[�g�ɂ���
			mState = EState::ESTART;
		}
		else if (mInput.Key('Y'))
		{
			mSoundSE.Play();
			mSoundOver.Stop();
			//�Q�[���̃C���X�^���X�폜
			delete mpGame;
			mpGame = nullptr;
			//�Q�[���̃C���X�^���X����
			mpGame = new CGame();
			//��Ԃ��X�^�[�g�ɂ���
			mState = EState::ESTAGE1;
			mSoundBGM.Repeat();
		}
		break;
	case EState::ECLEAR:
		//�Q�[���N���A����
		mpGame->Clear();
		if (mInput.Key(VK_RETURN))
		{
			mSoundSE.Play();
			mSoundClear.Stop();
			delete mpGame;
			mpGame = new CGame();
			mState = EState::ESTART;
		}
		break;
	}
}
CTexture CApplication::mTexture2;
CTexture* CApplication::Texture2()
{
	return &mTexture2;
}
CTexture CApplication::mTexture3;
CTexture* CApplication::Texture3()
{
	return &mTexture3;
}
CTexture CApplication::mTexture4;
CTexture* CApplication::Texture4()
{
	return &mTexture4;
}
CTexture CApplication::mTexture5;
CTexture* CApplication::Texture5()
{
	return &mTexture5;
}
CTexture CApplication::mTexture6;
CTexture* CApplication::Texture6()
{
	return &mTexture6;
}
CTexture CApplication::mTexture7;
CTexture* CApplication::Texture7()
{
	return &mTexture7;
}
CTexture CApplication::mTexture8;
CTexture* CApplication::Texture8()
{
	return &mTexture8;
}
CTexture CApplication::mTexture9;
CTexture* CApplication::Texture9()
{
	return &mTexture9;
}
CTexture CApplication::mTexture100;
CTexture* CApplication::Texture100()
{
	return &mTexture100;
}
CTexture CApplication::mTexture101;
CTexture* CApplication::Texture101()
{
	return &mTexture101;
}
CTexture CApplication::mTexture102;
CTexture* CApplication::Texture102()
{
	return &mTexture102;
}