#include"CPlayer2.h"
#include "CEnemy.h"
#include "CEnemy2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 158, 130 //�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 25.0f)   //�d�͉����x
#define JUMPV0 (TIPSIZE / 1.35f)    //�W�����v�̏���
#define HIGHJUMP0 (TIPSIZE / 0.6f)  //�n�C�W�����v�̏���
#define TEXCRY 196, 216, 158, 130   //�e�N�X�`���}�b�s���O
#define TEXCOORD2 136,156,158,130   //�E����2
#define TEXLEFT1 188,168,158,130    //������1
#define TEXLEFT2 156,136,158,130    //������2
#define SOUND_JUMP "res\\jump.wav"  //�W�����vSE
#define SOUND_DOWN "res\\down.wav"  //���݂�SE

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	:mInvincible(0)
	,interval(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	mspState = Espstate::NULLSTATE;
	sHp = 1;
	sgoal = 1;
	mSoundJump.Load(SOUND_JUMP);
	mSoundDown.Load(SOUND_DOWN);
	spInstance = this;
}

void CPlayer2::Update()
{
	CPlayer2::Move();
	if (interval > 0)
	{
		//�W�����v�̃C���^�[�o��
		interval--;
		if (interval < 25)
		{//������Ԃ��擾
			mState = EState::EFOLL;
		}
	}
	if (mInput.Key('W'))
	{//�W�����v��Ԃɂ���
		mState = EState::EJUMP;
		CPlayer2::Jump();
	}
	else if (mInput.Key('J'))
	{//�W�����v��Ԃɂ���
		mState = EState::EJUMP;
		CPlayer2::Jump();
	}
	if (mState == EState::EJUMP)
	{//�W�����v������Ăяo��
		CPlayer2::Jump();
	}
	if (mState == EState::ECRY)
	{//����������Ăяo��
		CPlayer2::Cry();
	}
	//������Ԃ̎��͉������Ȃ�
	if (mState == EState::EFOLL){}
	mspState = Espstate::NULLSTATE;
	//Y�����x�ɏd�͂����Z����
	mVy -= GRAVITY;
	//Y���W��Y�����x��������
	Y(Y() + mVy);
	{//�ʏ�̉摜��ݒ�
		Texture(Texture(), TEXCOORD);
	}

	if (mInvincible > 0)
	{
		//���G���Ԓ���1���Z����
		mInvincible--;
	}
	if (mInvincible > 0)
	{
		//�����摜��ݒ�
		Texture(Texture(), TEXCRY);
	}
	else
	{
		const int PITCH = 150;  //�摜��؂�ւ���Ԋu
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
		    else if (mVx < 0.0f) //���ֈړ�
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
					if (o->State() == EState::ECRY)
					{
						mVy = JUMPV0;
						mSoundDown.Play();
					}
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
	case ETag::EENEMY2:
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
					if (o->State() == EState::EMOVE)
					{
						mVy = HIGHJUMP0;
						mSoundDown.Play();
					}
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
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			mspState = Espstate::ESPMOVE;
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
			if (interval > 0)
			{
				if (y > 0)
				{
					mState = EState::EFOLL;
				}
			}
		}
		break;
	case ETag::EWALL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sHp--;
		}
		break;
	case ETag::EGOAL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sgoal--;
		}
		break;
	case ETag::EFOLLBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			//���n������
			if (y != 0.0f)
			{
				if (y > 0.0f)
				{
					mspState = Espstate::ESPMOVE;
					mState = EState::EMOVE;
				}
			}
		}
		break;
	case ETag::EDOKAN:
		if (CRectangle::Collision(o, &x, &y))
		{
			mspState = Espstate::ESPMOVE;
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
	}
}

int CPlayer2::sHp = 0;  //HP

int CPlayer2::Hp()
{
	return sHp;
}

int CPlayer2::sgoal = 0; //�S�[������l
int CPlayer2::Goal()
{
	return sgoal;
}

CPlayer2* CPlayer2::spInstance = nullptr;
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}

void CPlayer2::Jump()
{
	if (mState == EState::EJUMP)
	{
		if (interval == 0
			&& mspState == Espstate::ESPMOVE)
		{
			mVy = JUMPV0;
			mSoundJump.Play();
			interval = 10;
		}
	}
}

void CPlayer2::Move()
{
	//������ԈȊO�͈ړ��ł���
	if (mState != EState::ECRY)
	{
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
	}
}

void CPlayer2::Cry()
{
	//�����摜��ݒ�
	Texture(Texture(), TEXCRY);
	if (mInvincible == 0)
	{
		mInvincible = 60;
		sHp--;
	}
}