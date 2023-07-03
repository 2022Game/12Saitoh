#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
private:
	//�R���C�_�̐錾
	CCollider mColSpherHead;	//��
	CCollider mColSpherBody;	//��
	CCollider mColSpherSword0;	//��
	CCollider mColSpherSword1;	//��
	CCollider mColSpherSword2;	//��

public:
	//�R���X�g���N�^
	CXEnemy();

	//�R���C�_�̐ݒ�
	void Init(CModelX* model);
	//�X�V����
	void Update();

	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
};
#endif