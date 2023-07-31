#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"

/*
�R���C�_�N���X
�Փ˔���f�[�^
*/
class CCollisionManager;
class CCollider : public CTransform, public CTask
{
	friend CCollisionManager;
public:
	//�D��x�̕ύX
	virtual void ChangePriority();
	//�D��x�̕ύX
	void ChangePriority(int priorty);
	//CollisionTriangleLine(�O�p�R���C�_, �����R���C�_, �����l)
	//retrun:true�i�Փ˂��Ă���jfalse(�Փ˂��Ă��Ȃ�)
	//�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust, CVector* cross = nullptr);
	//CollisionTriangleLine(�O�p�`���_, �������_, �����l)
	//retrun:true�i�Փ˂��Ă���jfalse(�Փ˂��Ă��Ȃ�)
    //�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleLine(CVector(&tv)[3], CVector(&lv)[2], CVector* adjust, CVector* cross = nullptr);

	//CollisionTriangleSphere(�O�p�R���C�_,���R���C�_,�����l)
	// return:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	//�����l:�Փ˂��Ă��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* ajust);
	//�R���C�_�^�C�v
	enum EType
	{
		ESPHERE,	//���R���C�_
		ETRIANGLE,	//�O�p�R���C�_
		ELINE,		//�����R���C�_
	};
	EType Type();
	//�R���C�_�[�^�O
	enum class EColliderTag
	{
		ENULL,			//����
		EPLAYER,		//�v���C���[
		EENEMY,			//�G
		EALL,			//����
		EPLAYERBULLET,	//�v���C���[�̒e
		EENEMYBULLET,	//�G�̒e
		EFLAG,			//�t���b�O
	};
	EColliderTag ColliderTag();
	//�f�t�H���g�R���X�g���N�^
	CCollider();
	//�Փ˔���
	//Collision(�R���C�_1,�R���C�_2)
	//return::true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	static bool Collision(CCollider* m, CCollider* o);
	~CCollider();
	//�R���X�g���N�^
	//CCollider(�e�A�e�s��A�ʒu�A���a)
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius, EColliderTag tag);
	//�e�|�C���^�̎擾
	CCharacter3* Parent();
	//�`��
	void Render();
protected:
	EType mType; //�R���C�_�^�C�v
	EColliderTag mColliderTag;
	//���_
	CVector mV[3];
	CCharacter3* mpParent;//�e
	CMatrix* mpMatrix;//�e�s��
	float mRadius;//���a
};
#endif