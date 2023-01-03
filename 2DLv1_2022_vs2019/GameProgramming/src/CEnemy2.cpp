#include"CEnemy2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 190, 160 //�e�N�X�`���}�b�s���O
#define TEXCRY 196, 216, 190, 160   //�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 20.0f)   //�d�͉����x
#define TEXCOORD2 136,156,190,160   //�E����2
#define TEXLEFT1 188,168,190,160   //������1
#define TEXLEFT2 156,136,190,160  //������2

CEnemy2::CEnemy2(float x, float y, float w, float h, CTexture* pt)
	:cooltime(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY - 1;
	spInstance = this;
	cooltime = 30;
}

void CEnemy2::Update()
{
	const int PITCH = 32;  //�摜��؂�ւ���Ԋu
	if ((int)X() % PITCH < PITCH / 2)
		if (mVx > 0.0f)  //���ֈړ�
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
		if (mVx > 0.0f) //���ֈړ�
		{
			//������2��ݒ�
			Texture(Texture(), TEXLEFT2);
		}
		else
		{
			//2�Ԗڂ̉摜��ݒ�
			Texture(Texture(), TEXCOORD2);
		}
	//Y���W��Y�����x��������
	Y(Y() + mVy);
	//Y�����x�ɏd�͂����Z����
	mVy -= GRAVITY;
	if (mState == EState::ECRY)
	{
		cooltime--;
	}
	switch (mState)
	{
	case EState::ECRY:
		//�����摜��ݒ�
		Texture(Texture(), TEXCRY);
		//�N�[���^�C����0�ɂȂ��������
		if(cooltime == 0)
		mEnabled = false;
		break;
	case EState::EMOVE:
		if (CEnemy2::X() - CPlayer2::Instance()->X() < 1500)
		{
			//X�����x���AX���W���X�V����
			float x = X() - mVx;
			X(x);
		}
		break;
	}
}

void CEnemy2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}


void CEnemy2::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//�܂�Ԃ�����������
		if (CRectangle::Collision(o, &x, &y))
		{
			//�߂荞�܂Ȃ��ʒu�܂Ŗ߂�
			X(X() + x);
			Y(Y() + y);
			//X�����x�𔽓]������
			mVx = -mVx;
		}
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if ((o->State() == EState::EJUMP)||
				(o->State() == EState::EFOLL))
			{
				mState = EState::ECRY;
			}
			else if (o->State() == EState::ECRY)
			{
				mState = EState::EMOVE;
			}
		}
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			if (mVy < -mVy)
			{
				mVy = -10.0;
			}
			//���n������
			if (y != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVy = 0.0f;
				if (y > 0.0f)
				{
					if (mState != EState::ECRY)
					{
						mState = EState::EMOVE;
					}
				}
			}
		}
		break;
	case ETag::EFOLLBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			if (mVy < -mVy)
			{
				mVy = 0.0f;
			}
		}
	}
}

CEnemy2* CEnemy2::spInstance = nullptr;
CEnemy2* CEnemy2::Instance()
{
	return spInstance;
}