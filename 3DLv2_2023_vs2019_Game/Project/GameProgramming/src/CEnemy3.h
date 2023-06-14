#ifndef CENEMY3_H
#define CENEMY3_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
//�R���C�_�N���X�̃C���N���[�h
#include "CCollider.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy3 : public CCharacter3
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
	CEnemy3();
	//CEnemy3(�ʒu,��],�g�k)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//�X�V����
	void Update();

	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
};
#endif