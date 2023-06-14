#ifndef ALLY_H
#define ALL_Y
#include "CCharacter3.h"
#include "CCollider.h"
/*
�����N���X
*/
class CAlly : public CCharacter3
{
private:
	CVector mPoint;			//�ڕW�n�_
	CCollider mCollider1;	//���R���C�_
	CCollider mCollider2;	//���̃R���C�_
	CCollider mCollider3;	//�����g�R���C�_

	int mHp;				//�q�b�g�|�C���g

	static CModel sModel;	//���f���f�[�^

public:
	//�R���X�g���N�^
	CAlly();
	//CEnemy3(�ʒu,��],�g�k)
	CAlly(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//�X�V����
	void Update();

	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
};
#endif