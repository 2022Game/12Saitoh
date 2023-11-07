#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CRideableObject.h"
#include "CSword.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CXCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �ҋ@���
	void Update_Idle();
	// �ړ����
	void Update_Move();
	//�_�b�V���ړ����
	void Update_FastMove();
	// �U��
	void Update_Attack();
	// ���̍U���҂�
	void Update_AttackWait();
	// �U���I��
	void Update_AttackEnd();
	// �W�����v�J�n
	void Update_JumpStart();
	// �W�����v��
	void Update_Jump();
	// �W�����v�I��
	void Update_JumpEnd();
	// ��𓮍�
	void Update_Avoidance();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

	// �[����Ԃ�������Ԃ�����
	bool IsDrawn();
private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,					// T�|�[�Y
		eIdle,					// �ҋ@(�[��)
		eIdle_Combat,			// �ҋ@(����)
		eIdleAir,				// �󒆑ҋ@(�[��)
		eIdleAir_Combat,		// �󒆑ҋ@(����)
		eIdle_Drawn_Combat,		// �ҋ@����������
		eIdle_Sheathed_Combat,	// �ҋ@���[������
		eRunStart,				// ����J�n(�[��)
		eRun,					// ����(�[��)
		eRunEnd,				// ����I��(�[��)
		eRunStart_Combat,		// ����J�n(����)
		eRun_Combat,			// ����(����)
		eRunEnd_Combat,			// ����I��(����)
		eRun_Drawn_Combat,		// ���蒆��������
		eRun_Sheathed_Combat,	// ���蒆�[������
		eFastRunStart,			// �_�b�V���J�n
		eFastRun,				// �_�b�V��
		eFastRunEnd,			// �_�b�V���I��
		eRollStart,				// ��𓮍�J�n
		eRollEnd_idle,			// �����A�C�h���ڍs
		eRollEnd_run,			// ����㑖��ڍs
		eRunAttack,				// �����U��
		eNormalAttack1_1,		// �ʏ�U��1-1
		eNormalWait1_1,			// �ʏ�U��1-1�U���҂�
		eNormalEnd1_1,			// �ʏ�U��1-1�I��
		eNormalAttack1_2,		// �ʏ�U��1-2
		eNormalWait1_2,			// �ʏ�U��1-2�U���҂�
		eNormalEnd1_2,			// �ʏ�U��1-2�I��
		eNormalAttack1_3,		// �ʏ�U��1-3
		eNormalWait1_3,			// �ʏ�U��1-3�U���҂�
		eNormalEnd1_3,			// �ʏ�U��1-3�I��
		eAirAttack1_1,			// �󒆍U��1-1
		eAirAttackWait1_1,		// �󒆍U��1-1�U���҂�
		eAirAttackEnd1_1,		// �󒆍U��1-1�I��
		eAttack_Up,				// �W�����v�U��
		eLanding,				// ���n����(�[��)
		eLandin_Combat,			// ���n����(����)
		eJumpStart,				// �W�����v�J�n
		eJump,					// �W�����v��
		eJumpEnd,				// �W�����v�I��
		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���[��Ԃ�؂�ւ���
	void SwitchDrawn();
	// ���[�̐؂�ւ�����
	void Update_SwitchDrawn();

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �v���C���[�̏��
	enum class EState
	{
		None = -1,	//�����l

		eIdle,		// �ҋ@
		eMove,		// �ړ�
		eFastMove,	// �_�b�V���ړ�
		eAvoidance,	// ���
		eAttack,	// �U��
		eAttackWait,// �U���I���҂�
		eAttackEnd,	// �U���I��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	EState mState;		// �v���C���[�̏��

	CVector mInput_save;// ���̓x�N�g�������ۑ�
	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsDrawn;		// �[����Ԃ�������Ԃ����� true:���� false:�[��
	
	CColliderLine* mpColliderLine;
	CTransform* mpRideObject;
	CSword* mpSword;
};
