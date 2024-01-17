#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderLine.h"
#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �C���X�^���X���擾
	static CEnemy* Instance();
	// �R���X�g���N�^
	CEnemy();

	// �X�V����
	void Update();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`�揈��
	void Render();

private:
	// �ҋ@���
	void Update_Idle();
	// �ړ����
	void Update_Move();
	// �U�����
	void Update_Attack();
	// ���S���
	void Update_Death();

	// �R���C�_�[�̍X�V����
	void ColliderUpdate();

	// �v���C���[�����������ǂ���
	bool IsFoundPlayer()const;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �A�C�h��
		eIdle2,		// �A�C�h��2
		eIdle3,		// �A�C�h��3
		eWalk,		// ����
		eRun,		// ����
		eAttack_Bite,// ���݂�
		eAttack_Scratching,// �����~��
		eAttack_Tail,// �K���U��
		eRoar,		// ���K
		eFear_Right,// ����(�E)
		eFear_Left,	// ����(��)
		eDeath,		// ���S
		eFly,		// ��s
		eFlyStart,	// ��s�n��
		eFly_Attack,// ��s���U��
		eFly_Breath,// ��s���u���X
		eFly_Fear,	// ��s������
		eFly_Death,	// ��s�����S

		Num
	};
	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EAnimType type);
	// �A�j���[�V�����f�[�^�e�[�u��
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float motionValue;	// ���[�V�����l
	};
	
	// �G�̏��
	enum class EState
	{
		None = -1,

		eIdle,		// �ҋ@
		eMove,		// �ړ�
		eAttack,	// �U��
		eDeath,		// ���S
	};
	EState mState;	// �G�̏��

	// �A�j���[�V�����f�[�^�e�[�u��
	static const AnimData ANIM_DATA[];
	// �C���X�^���X
	static CEnemy* spInstance;

	CModel* mModel;
	CColliderSphere* mpHeadCol;		// ��
	CColliderSphere* mpBodyCol;		// ��
	CColliderSphere* mpTailCol;		// �K��
	CColliderLine* mpColliderLine;	// �n�ʂƂ̓����蔻��p

	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	int mIdleTime;		// �A�C�h���Ǘ��p�̎���
	float mElapsedTime;	// �o�ߎ��Ԍv����
};
#endif
