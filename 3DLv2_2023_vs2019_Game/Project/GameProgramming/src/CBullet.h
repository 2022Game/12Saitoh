#ifndef CBULLET_H
#define CBULLET_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
//�O�p�`�N���X�̃C���N���[�h
#include "CTriangle.h"
//�R���C�_�N���X�̃C���N���[�h
#include "CCollider.h"
#include "CEnemy3.h"
/*
�e�N���X
�O�p�`���΂�
*/
class CBullet : public CCharacter3 
{
	friend CEnemy3;
private:
	CCollider mCollider;
	CVector mV;	//�������˗p
	CModel mModelBall;
	static CBullet* spinstence;

public:
	CBullet();
	//���Ɖ��s���̐ݒ�
	//Set(��,���s)
	void Set(float w, float d);
	//�X�V
	void Update();
	//�`��
	void Render();
	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	static CBullet* Instance();
};
#endif;