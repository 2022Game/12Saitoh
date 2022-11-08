#include"CPlayer2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 158, 128 //�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 25.0f)   //�d�͉����x
#define JUMPV0 (TIPSIZE / 1.35f)     //�W�����v�̏���
#define TEXCRY 196, 216, 158, 128   //�e�N�X�`���}�b�s���O
#define TEXCOORD2 136,156,158,128   //�E����2
#define TEXLEFT1 188,168,158,128    //������1
#define TEXLEFT2 156,136,158,128    //������2
#define SOUND_JUMP "res\\jump.wav"  //�W�����vSE
//#define VELOCITY 4.0f             //�ړ����x
//#define HP 3                      //HP�̏����l��3

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	:mInvincible(0)
	,interval(0)
	//,mVy(0.0f)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	sHp = 1;
	mSoundJump.Load(SOUND_JUMP);
	spInstance = this;
}

void CPlayer2::Update()
{
	if (interval > 0)
	{
		mState = EState::EJUMP;
		//�W�����v�̃C���^�[�o��
		interval--;
	}
	if (mInvincible > 0)
	{
		//���G���Ԓ���1���Z����
		mInvincible--;
	}
	if (mInput.Key('A'))
	{
		mVx = -VELOCITY - 1;
		X(X() + mVx + mVx);
	}
	if (mInput.Key('D'))
	{
		mVx = VELOCITY + 1;
		X(X() + mVx + mVx);
	}
	if (mState != EState::EJUMP)
	{
		if (interval == 0)
		{
			if (mInput.Key('W'))
			{
				mVy = JUMPV0;
				mSoundJump.Play();
				mState = EState::EJUMP;
				interval = 35;
			}
		}
	}

	//Y���W��Y�����x��������
	Y(Y() + mVy);
	//Y�����x�ɏd�͂����Z����
	mVy -= GRAVITY;
	{//�ʏ�̉摜��ݒ�
		Texture(Texture(), TEXCOORD);
	}

	if (mState == EState::ECRY)
	{
		//�����摜��ݒ�
		Texture(Texture(), TEXCRY);
		if (mInvincible == 0)
		{
			mInvincible = 60;
			sHp--;
		}
	}
	if (mInvincible > 0)
	{
		//�����摜��ݒ�
		Texture(Texture(), TEXCRY);
	}
	else
	{
		const int PITCH = 100;  //�摜��؂�ւ���Ԋu
		if ((int)X() % PITCH < PITCH / 2)
			if (mVx < 0.0f)  //���ֈړ�
			{
				//������1��ݒ�
				Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//�ʏ�̉摜��ݒ�
				Texture(Texture(), TEXCOORD);
			}
		else
			if (mVx < 0.0f) //���ֈړ�
			{
				//������2��ݒ�
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				Texture(Texture(), TEXCOORD2);
			}
	}
}

void CPlayer2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CPlayer2::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o, &x, &y))
		{
			//�G�̏Փ˔�������s
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//���n������
			if (y != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EJUMP;
					mVy = JUMPV0;
				}
				else
				{//�W�����v�łȂ���΋���
					mState = EState::ECRY;
				}
			}
			else
			{//�W�����v�łȂ���΋���
				mState = EState::ECRY;
			}
		}
		break;
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			//���n������
			if (y != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVy = 0.0f;
					if (y > 0.0f)
					{
					mState = EState::EMOVE;
					}
			}
		}
		break;
	case ETag::ETRAP:
		if (CRectangle::Collision(o, &x, &y))
		{
			if (y > 0)
			{
				//mState = EState::EMOVE;
			}
		}
		break;
	case ETag::EWALL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sHp--;
		}
		break;
	}
}

int CPlayer2::sHp = 0;  //HP

int CPlayer2::Hp()
{
	return sHp;
}

CPlayer2* CPlayer2::spInstance = nullptr;
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}
