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
class NavNode;
class CEnemy : public CCharacter3
{
private:
	//�G�̏��
	enum class EState
	{
		EIDLE,		//�ҋ@
		ECHASE,		//�ǐ�
		ELOST,		//��������
		EATTACK		//�U��
	};
	//�G�̏��
	EState mState;

	CCollider mCollider1;	//���R���C�_
	CCollider mCollider2;	//���̃R���C�_
	CCollider mCollider3;	//�����g�R���C�_
	NavNode* mpNextNode;
	NavNode* mpLostNode;

	static CEnemy* spInstance;
	static CModel sModel;	//���f���f�[�^
	static int sNum;		//�G�̐�

	int mHp;				//�q�b�g�|�C���g
	int mBulletTime;		//�ˌ��Ԋu(����)
	int mIdleTime;			//�ҋ@���̓�������߂�
	int mRandIdle;			//�����_���l��ۑ�

	//��ʂ��΂��Ă��邩�̔���t���O
	bool mFlag;

	//�v���C���[�����������ǂ���
	bool IsFoundPlayer() const;
	//�U���ł���͈͓����̔���
	bool AttackRange() const;
	//�w��̍��W�܂ňړ�
	bool MoveTo(const CVector& target);

	//�ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	//�ǐՏ�Ԃ̍X�V����
	void UpdateChase();
	//����������Ԃ̍X�V����
	void UpdateLost();
	//�U����Ԃ̍X�V����
	void UpdateAttack();

public:
	//�R���X�g���N�^
	CEnemy();
	//CEnemy(�ʒu,��],�g�k)
	CEnemy(const CVector& position, const CVector& rotation,
		const CVector& scale);
	//�f�X�g���N�^
	~CEnemy();
	//�C���X�^���X�̃|�C���^�̎擾
	static CEnemy* Instance();

	//�G�̐���������
	int ResetNum();
	//�G�̐����擾
	int Num()const;

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