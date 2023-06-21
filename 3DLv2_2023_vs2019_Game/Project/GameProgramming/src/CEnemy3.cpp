#include "CEnemy3.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define HP 3               //�ϋv�l
#define VELOCITY 0.1f      //���x
#define OBJ "res\\SnowGolem.obj" //���f���̃t�@�C��
#define MTL "res\\SnowGolem.mtl" //���f���̃}�e���A���t�@�C��
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//�d��
#define FOV_ANGLE 45.0f	//����̊p�x (-�p�x�`+�p�x�܂�)
#define FOV_LENGTH 30.0f	//����̋���

float CEnemy3::mDotX = 0.0f;
CModel CEnemy3::sModel;    //���f���f�[�^�쐬

//�v���C���[�����������ǂ���
bool CEnemy3::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	//����̊p�x�̔���
	//���g����v���C���[�܂ł̃x�N�g�����擾(�v���K��)
	CVector EP = (playerPos - enemyPos).Normalize();
	//���g�̐��ʕ����̃x�N�g�����擾(�v���K��)
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//���g�̉������̃x�N�g�����擾
	CVector side = MatrixRotate().VectorX().Normalize();
	//���ʕ����̃x�N�g���ƃv���C���[�܂ł̃x�N�g����
	//���ς���p�x�����߂�
	float dotZ = forward.Dot(EP);
	//�������̃x�N�g���̓��ς���p�x�����߂�
	float dotX = side.Dot(EP);
	mDotX = dotX;
	//���߂��p�x������p�x�O�ŗL��΁Afalse��Ԃ�
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	//���싗���̔���
	//���g����v���C���[�܂ł̋��������߂�
	float distance = (playerPos - enemyPos).Length();
	//���߂����������싗����艓���Ȃ�΁Afalse��Ԃ�
	if (distance > FOV_LENGTH)return false;

	//����p�x�Ǝ��싗����ʂ����̂�true��Ԃ�
	return true;
}

//�f�t�H���g�R���X�g���N�^
CEnemy3::CEnemy3()
	: CCharacter3(1)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, (int)EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, (int)EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, (int)EColliderTag::EENEMY)
	, mHp(HP)
	, mBulletTime(0)
	, mFlag(false)
	, mState(EState::EIDLE)
{
	//���f�����������͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���f�[�^�̃|�C���^�ݒ�
	mpModel = &sModel;
}

//�R���X�g���N�^
//CEnemy3(�ʒu,��],�g�k)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy3() //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu,��],�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̍X�V
}

//�ҋ@��Ԃ̍X�V����
void CEnemy3::UpdateIdle()
{
	//�v���C���[����������A�ǐՏ�Ԃ��ڍs
	if (IsFoundPlayer())
	{
		mState = EState::ECHASE;
	}
}

//�ǐՏ�Ԃ̍X�V����
void CEnemy3::UpdateChase()
{
	//�v���C���[������������A�ҋ@��Ԃֈڍs
	if (!IsFoundPlayer())
	{
		mState = EState::EIDLE;
	}
	//�v���C���[���������Ă��Ȃ���΁A�ړ�����
	else
	{
		CVector forward = mMatrixRotate.VectorZ().Normalize();
		CVector playerPos = CPlayer::Instance()->Position();
		float distance = (playerPos - Position()).Length();
		if (distance >= 5.0f)
		{
			mPosition += forward * VELOCITY;
		}
		else if (distance <= 3.0f)
		{
			mPosition -= forward * VELOCITY;
		}
		//������3�`5�̊Ԃł���΁A�U����Ԃֈڍs
		else
		{
			mState = EState::EATTACK;
		}
		//���E�����։�]
		if (mDotX >= 0.0f)
		{
			mRotation += CVector(0.0f, 1.5f, 0.0f); //���։�]
		}
		else
		{
			mRotation -= CVector(0.0f, -1.5f, 0.0f); //�E�։�]
		}
	}
}

//�U����Ԃ̍X�V����
void CEnemy3::UpdateAttack()
{
	CVector playerPos = CPlayer::Instance()->Position();
	float distance = (playerPos - Position()).Length();

	//�U������
	//��ʂ𔭎˂��Ă��Ȃ����
	if (mFlag == false)
	{
		//�e�𔭎�
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->Position(CVector(0.0f, 70.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		mFlag = true;
		printf("�e����\n");
	}
	//��e���ˍς�
	else
	{
		mBulletTime++;
		if (mBulletTime >= 100)
		{
			if (mFlag == true)
			{
				mFlag = false;
				mBulletTime = 0;
				//�U�����I�������ǐՏ�Ԃɖ߂�
				mState = EState::ECHASE;
			}
		}
	}

}

//�X�V����
void CEnemy3::Update()
{
	//�d��
	mPosition = mPosition - GRAVITY;

	switch (mState)
	{
	case EState::EIDLE:
		UpdateIdle();
		break;
	case EState::ECHASE:
		UpdateChase();
		break;
	case EState::EATTACK:
		UpdateAttack();
		break;
	}


	//HP��0�ȉ��̎��A���j
	if (mHp <= 0)
	{
		mHp--;
		//15�t���[�����ɃG�t�F�N�g
		if (mHp % 15 == 0)
		{
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//�~��������
		mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
		CTransform::Update();
		return;
	}

	CTransform::Update(); //�s��X�V
}


void CEnemy3::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			if (o->GetTag() != (int)EColliderTag::EENEMY &&
				o->GetTag() != (int)EColliderTag::EALL)
			{
				mHp--; //�q�b�g�|�C���g�̌��Z
				//�G�t�F�N�g����
				new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//�Փ˂��Ă��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
			//���j�Œn�ʂɏՓ˂���Ɩ���
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}
}