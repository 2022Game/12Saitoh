//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include"CApplication.h"
#include"CTaskManager.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //��]���x
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //��]���x
#define VELOCITYZ CVector(0.0f, 0.0f, 0.2f) //Z���ړ����x
#define VELOCITYX CVector(0.2f, 0.0f, 0.0f)	//X���ړ����x
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//�d��

CPlayer::CPlayer()
	: mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, (int)EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, (int)EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, (int)EColliderTag::EENEMY)

{
	//�C���X�^���X�̐ݒ�
	spInstance = this;
}

//CPlayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
	mPosition = mPosition - GRAVITY;
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.PushKey(VK_SPACE))
	{
		CBullet *bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 70.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
	}
	//W�L�[���͂ŏ����
	if (mInput.Key('W'))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITYZ * mMatrixRotate;
	}
	//S�L�[���͂ŏ����
	if (mInput.Key('S'))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition - VELOCITYZ * mMatrixRotate;
	}
	if (CApplication::CameraFlag() != 2)
	{
		if (mInput.Key('A'))
		{
			//X�������̒l����]�����ړ�������
			mPosition = mPosition + VELOCITYX * mMatrixRotate;
		}
		//D�L�[���͂ŉ�]
		if (mInput.Key('D'))
		{
			//X�������̒l����]�����ړ�������
			mPosition = mPosition - VELOCITYX * mMatrixRotate;
		}
	}
	else
	{
		if (mInput.Key('A'))
		{
			//X�������̒l����]�����ړ�������
			mPosition = mPosition - VELOCITYX * mMatrixRotate;
		}
		//D�L�[���͂ŉ�]
		if (mInput.Key('D'))
		{
			//X�������̒l����]�����ړ�������
			mPosition = mPosition + VELOCITYX * mMatrixRotate;
		}
	}
	if (mInput.Key(VK_RIGHT))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation - ROTATION_YV;
	}
	if (mInput.Key(VK_LEFT))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation + ROTATION_YV;
	}
	if (mInput.Key(VK_UP))
	{
		mRotation = mRotation - ROTATION_XV;
	}
	if (mInput.Key(VK_DOWN))
	{
		mRotation = mRotation + ROTATION_XV;
	}

	//�ϊ��s��̍X�V
	CTransform::Update();
	//Ui�ݒ�
	CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	CVector adjust; //�����l
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{

		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�

		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//�Փ˂��Ă��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
		}
		break;
	}
}

//�Փˏ���
void CPlayer::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
	////�R���C�_�̗D��x�ύX
	//mLine.ChangePriority();
	//mLine2.ChangePriority();
	//mLine3.ChangePriority();
	////�Փˏ��������s
	//CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

CPlayer* CPlayer::spInstance = nullptr;
CPlayer* CPlayer::Instance()
{
	return spInstance;
}