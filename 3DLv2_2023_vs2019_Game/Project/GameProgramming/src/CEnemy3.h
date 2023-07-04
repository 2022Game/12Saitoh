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
	CCollider mCollider1;	//���R���C�_
	CCollider mCollider2;	//���̃R���C�_
	CCollider mCollider3;	//�����g�R���C�_

	int mHp;				//�q�b�g�|�C���g
	int mBulletTime;		//�ˌ��Ԋu(����)

	static float mDotX;		//�������̊p�x
	static CModel sModel;	//���f���f�[�^

	//�v���C���[�����������ǂ���
	bool IsFoundPlayer() const;
	bool mFlag;				//��ʂ��΂��Ă��邩�̔���t���O

	//�G�̏��
	enum class EState
	{
		EIDLE,		//�ҋ@
		ECHASE,		//�v���C���[��ǐ�
		EATTACK		//�v���C���[���U��
	};
	//�G�̏��
	EState mState;
	//�ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	//�ǐՏ�Ԃ̍X�V����
	void UpdateChase();
	//�U����Ԃ̍X�V����
	void UpdateAttack();

public:
	//�R���X�g���N�^
	CEnemy3();
	//CEnemy3(�ʒu,��],�g�k)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//�X�V����
	void Update();
	//�`�揈��
	void Render();

	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);

};
#endif