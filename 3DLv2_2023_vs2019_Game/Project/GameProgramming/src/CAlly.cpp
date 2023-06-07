#include "CAlly.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"

#define HP 3						//�ϋv�l
#define VELOCITY 0.1f				//���x
#define OBJ "res\\SnowGolem.obj"	//���f���̃t�@�C��
#define MTL "res\\SnowGolem.mtl"	//���f���̃}�e���A���t�@�C��

CModel CAlly::sModel;				//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CAlly::CAlly()
	:CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f)
	, mHp(HP)
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
//CAlly(�ʒu,��],�g�k)
CAlly::CAlly(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CAlly() //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu,��],�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//�X�V����
void CAlly::Update()
{
	//�v���C���[�̃|�C���^��0�ȊO�̎�
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//�ڕW�n�_�܂ł̃x�N�g�������߂�
		CVector vp = mPoint - mPosition;
		//�v���C���[�܂ł̃x�N�g�������߂�
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.VectorY());
		//�O�����x�N�g���̓��ς����߂�
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		const float margin = 0.1f;

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
		//���悻3�b���ɖڕW�n�_���X�V
		int r = rand() % 180; //rand()�͐����̗�����Ԃ�
							  //% 180��180�Ŋ������]������߂�
		if (r == 0)
		{
			if (player != nullptr)
			{
				mPoint = player->Position();
			}
			else
			{
				mPoint = mPoint * CMatrix().RotateY(45);
			}
		}
		//���E�����։�]
		if (dx > margin)
		{
			mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f); //���։�]
		}
		else if (dx < -margin)
		{
			mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f); //�E�։�]
		}
		//�@�̑O���ֈړ�����
		mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY;
		CTransform::Update(); //�s��X�V
	}
}


void CAlly::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CAlly::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			mHp--; //�q�b�g�|�C���g�̌��Z
			//�G�t�F�N�g����
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ă��鎞�͖����ɂ���
			//mEnabled = false;
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