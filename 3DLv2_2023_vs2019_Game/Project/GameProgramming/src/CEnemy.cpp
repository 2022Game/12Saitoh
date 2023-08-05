#include "CEnemy.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Math.h"
#include <time.h>
#include "Primitive.h"
#include "NavManager.h"
#include "Global.h"
#include "CCollisionManager.h"
#include "CEnemyBullet.h"

#define HP 3               //�ϋv�l
#define VELOCITY 0.1f      //���x
#define OBJ "res\\SnowGolem.obj" //���f���̃t�@�C��
#define MTL "res\\SnowGolem.mtl" //���f���̃}�e���A���t�@�C��
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//�d��
#define MOVE_SPEED 0.125f		//�ړ����x
#define FOV_ANGLE 80.0f			//����̊p�x (-�p�x�`+�p�x�܂�)
#define FOV_ATTACKANGLE 5.0f	//�U���͈�(�p�x)
#define FOV_LENGTH 18.0f		//����̋���
#define FOV_LANGE 10.0f			//�F���͈�
#define FOV_ATTACKLENGTH 15.0f	//�U���͈�(����)

CModel CEnemy::sModel;	//���f���f�[�^�쐬
int CEnemy::sNum = 0;	//������
CEnemy* CEnemy::spInstance = nullptr;

//�v���C���[�����������ǂ���
bool CEnemy::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	//����̊p�x�̔���
	//���g����v���C���[�܂ł̃x�N�g�����擾(�v���K��)
	CVector EP = (playerPos - enemyPos).Normalize();
	//���g�̐��ʕ����̃x�N�g�����擾(�v���K��)
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//���ʕ����̃x�N�g���ƃv���C���[�܂ł̃x�N�g����
	//���ς���p�x�����߂�
	float dotZ = forward.Dot(EP);
	//���g����v���C���[�܂ł̋��������߂�
	float distance = (playerPos - enemyPos).Length();
	//���̋����ɓ�������true��Ԃ�
	if (distance < FOV_LANGE)return true;

	//���߂��p�x������p�x�O�ŗL��΁Afalse��Ԃ�
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	//���싗���̔���
	//���߂����������싗����艓���Ȃ�΁Afalse��Ԃ�
	if (distance > FOV_LENGTH)return false;

	//����p�x�Ǝ��싗����ʂ����̂�true��Ԃ�
	return true;
}

//�U���ł���͈͓����̔���
bool CEnemy::AttackRange() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = CEnemy::Position();

	//�U���͈�(�p�x)�̔���
	//���g����v���C���[�܂ł̃x�N�g�����擾
	CVector EP = (playerPos - enemyPos).Normalize();
	//���g���琳�ʕ����̃x�N�g�����擾
	CVector forward = MatrixRotate().VectorZ().Normalize();
	//���ʕ����̃x�N�g���ƃv���C���[�܂ł�
	//�x�N�g���̓��ς���p�x�����߂�
	float dotZ = forward.Dot(EP);
	//���߂��p�x������͈͊O��������false��Ԃ�
	if (dotZ <= cosf(FOV_ATTACKANGLE * M_PI / 180.0f)) return false;

	//�U���͈�(����)�̔���
	float distance = (playerPos - enemyPos).Length();
	//���߂��������U���͈͂�艓���Ȃ�false��Ԃ�
	if (distance > FOV_ATTACKLENGTH) return false;

	//�U���͈͓��Ȃ̂�true��Ԃ�
	return true;
}

//�w����W�܂ňړ�
bool CEnemy::MoveTo(const CVector& target)
{
	//�ړI�n�܂ł̋������ړ����x���傫���Ȃ��
	float distance = (target - Position()).Length();
	if (distance > MOVE_SPEED)
	{
		//�ړI�n�����ֈړ�
		CVector moveVec = (target - Position()).Normalize();
		Position(Position() + moveVec * MOVE_SPEED);

		//�ړI�n�����֌���
		float currentAngle = mRotation.Y();
		float targetAngle = atan2f(moveVec.X(), moveVec.Z());
		targetAngle = Math::RadianToDegree(targetAngle);

		//��������]���鎞�ɍ��E�ŋ߂��������]����
		float diff = targetAngle - currentAngle;
		if (diff > 180.0f)
		{
			targetAngle -= 360.0f;
		}
		else if (diff < -180.0f)
		{
			currentAngle -= 360.0f;
		}

		float angle = Math::Lerp(currentAngle, targetAngle, 0.1f);
		mRotation.Y(angle);
	}
	//�������ړ����x���Z���̂ł���΁A
	else
	{
		//�ړI�n�̍��W�ɔz�u
		Position(target);
		//�ړ��I��
		return true;
	}

	//�ړ��p��
	return false;
}

//�ҋ@��Ԃ̍X�V����
void CEnemy::UpdateIdle()
{
	//�v���C���[����������A�ǐՏ�ԂɈڍs
	if (IsFoundPlayer())
	{
		mState = EState::ECHASE;
	}

	//150�t���[�����ɑҋ@���̓�����X�V����
	mIdleTime++;
	if (mIdleTime >= 150)
	{
		//�����_���l�̎擾
		srand((unsigned int)time(NULL));
		//�����_���l��ۑ�����
		//�ŏ��l0 ,�擾�͈�6
		 mRandIdle = 0 + rand() % 6;
	}
	//�X�V�̏�����
	if (mIdleTime == 150)
		mIdleTime = 0;
	switch (mRandIdle)
	{
	case 0:
		//�O�����ֈړ�
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 1:
		//�E�����ֈړ�
		Rotation(mRotation - CVector(0.0f, 1.0f, 0.0f));
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 2://�������ֈړ�
		Rotation(mRotation + CVector(0.0f, 1.0f, 0.0f));
		Position(Position() + (CVector(0.0f, 0.0f, 1.0f)
			* MOVE_SPEED) * mMatrixRotate);
		break;
	case 3://�E�։�]
		Rotation(mRotation - CVector(0.0f, 1.0f, 0.0f));
		break;
	case 4://���։�]
		Rotation(mRotation + CVector(0.0f, 1.0f, 0.0f));
		break;
	case 6://�����Ȃ�
		break;
	}
}

//�ǐՏ�Ԃ̍X�V����
void CEnemy::UpdateChase()
{
	//�v���C���[�̃m�[�h���擾
	NavNode* playerNode = CPlayer::Instance()->GetNavNode();
	//���g�̃m�[�h���擾
	NavNode* enemyNode = mpNode;

	CVector playerNodePos = playerNode->GetPos();
	CVector enemyNodePos = enemyNode->GetPos();
	//�v���C���[�܂ł̃x�N�g��
	CVector vec = playerNodePos - enemyNodePos;
	//�v���C���[�܂ł̋���
	float dist = vec.Length();

	//�v���C���[�܂ł̊ԂɎՕ��������邩���ׂ�
	float outDist = 9999.0f;
	//���C�̓����蔻��͖ڐ�������łƂ�
	gMap->CollisionRay(enemyNodePos + CVector(0.0f, 0.5f, 0.0f), playerNodePos + CVector(0.0f, 0.5f, 0.0f), &outDist);
	//�Օ��������݂��Ȃ�
	if (dist < outDist)
	{
		//���g�̃m�[�h����v���C���[�̃m�[�h�܂ł�
		//�o�H�T�����s��
		mpNextNode = NavManager::Instance()->Navigate(enemyNode, playerNode);
		//�v���C���[�������������̃m�[�h���X�V
		if (mpLostNode == nullptr)
		{
			mpLostNode = new NavNode(playerNodePos);
		}
		else
		{
			mpLostNode->SetPos(playerNodePos);
		}
	}
	//�Օ���������
	else
	{
		//���݂̃v���C���[�m�[�h�܂ł̌o�H��T������
		//����������Ԃֈڍs����
		enemyNode->RemoveConnect(playerNode);
		mpNextNode = NavManager::Instance()->Navigate(enemyNode, mpLostNode);
		mState = EState::ELOST;
		return;
	}
	//���̖ړI�n�����݂���
	if (mpNextNode != nullptr)
	{
		//���̖ړI�n�܂ňړ�
		MoveTo(mpNextNode->GetPos());
	}
	//���̖ړI�n�����݂��Ȃ��ꍇ�́A�ړI�n����
	else
	{
		//�ҋ@��Ԃֈڍs
		mState = EState::EIDLE;
	}
	//�v���C���[���͈͓��ɂ���΍U������
	if (AttackRange())
	{
		mState = EState::EATTACK;
	}
}

//����������Ԃ̍X�V����
void CEnemy::UpdateLost()
{
	//�ړI�n�����݂���
	if (mpNextNode != nullptr)
	{
		//�ړI�n�܂ňړ�
		if (MoveTo(mpNextNode->GetPos()))
		{
			if (mpNextNode != mpLostNode)
			{
				mpNextNode = NavManager::Instance()->Navigate(mpNextNode, mpLostNode);
			}
			else
			{
				delete mpLostNode;
				mpLostNode = nullptr;
				mpNextNode = nullptr;
			}
		}
	}
	//�ړI�n�܂ňړ����I���΁A
	else
	{
		//�ҋ@��Ԃֈڍs
		mState = EState::EIDLE;
	}
	//�v���C���[���͈͓��ɂ���΍U������
	if (AttackRange())
	{
		mState = EState::EATTACK;
	}
}

//�U����Ԃ̍X�V����
void CEnemy::UpdateAttack()
{
	//�U������
	//��ʂ𔭎˂��Ă��Ȃ����
	if (mFlag == false)
	{
		//�e�𔭎�
		CEnemyBullet* bullet = new CEnemyBullet(CCollider::EColliderTag::EENEMYBULLET);
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
		if (mBulletTime >= 100 && mFlag)
		{
			mFlag = false;
			mBulletTime = 0;
			//�U�����I�������ǐՏ�Ԃɖ߂�
			mState = EState::EIDLE;
		}
	}

}

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
	: CCharacter3(1)
	, mState(EState::EIDLE)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 1.05, CCollider::EColliderTag::EENEMY)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.54, CCollider::EColliderTag::EENEMY)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.675, CCollider::EColliderTag::EENEMY)
	, mpNextNode(nullptr)
	, mpLostNode(nullptr)
	, mHp(HP)
	, mBulletTime(0)
	, mIdleTime(0)
	, mRandIdle(-1)
	, mFlag(false)
{
	//���f�����������͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���f�[�^�̃|�C���^�ݒ�
	mpModel = &sModel;
	mpNode = new NavNode(Position());
	spInstance = this;
	sNum++;
}

//�R���X�g���N�^
//CEnemy(�ʒu,��],�g�k)
CEnemy::CEnemy(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy() //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu,��],�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̍X�V
}

//�f�X�g���N�^
CEnemy::~CEnemy()
{
	if (mpNode != nullptr)
	{
		delete mpNode;
		mpNode = nullptr;
	}
}

//�C���X�^���X�̃|�C���^�̎擾
CEnemy* CEnemy::Instance()
{
	return spInstance;
}

//�G�̐���������
int CEnemy::ResetNum()
{
	return sNum = 0;
}

//�G�̐��̎擾
int CEnemy::Num()const
{
	return sNum;
}

//�X�V����
void CEnemy::Update()
{
	//�d��
	mPosition = mPosition - GRAVITY;

	switch (mState)
	{
	case EState::EIDLE://�ҋ@
		UpdateIdle();
		break;
	case EState::ECHASE://�ǐ�
		UpdateChase();
		break;
	case EState::ELOST://��������
		UpdateLost();
		break;
	case EState::EATTACK://�U��
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
	//�s��̍X�V
	CTransform::Update();
	CCharacter3::Update();
}

//�`�揈��
void CEnemy::Render()
{
	CCharacter3::Render();

	Primitive::DrawSector
	(
		mPosition,
		mRotation,
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LENGTH,
		CVector4(1.0f, 1.0f, 0.0f, 1.0f),
		45
	);
	Primitive::DrawSector
	(
		mPosition + CVector (0.0f,0.1f,0.0f),
		mRotation,
		-FOV_ATTACKANGLE,
		FOV_ATTACKANGLE,
		FOV_ATTACKLENGTH,
		CVector4(1.0f, 1.0f, 1.0f, 1.0f),
		45
	);
}


void CEnemy::Collision()
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
void CEnemy::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			//�v���C���[�̒e�̎�HP�����Z����
			if (o->ColliderTag() == CCollider::EColliderTag::EPLAYERBULLET)
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
			//�G�̐���1���炷
			if (mHp <= 0)
			{
				mEnabled = false;
				sNum--;
			}
		}
		break;
	}
}
