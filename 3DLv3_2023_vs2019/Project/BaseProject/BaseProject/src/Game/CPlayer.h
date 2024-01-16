#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CRideableObject.h"
#include "CSword.h"
#include "PlayerData.h"
#include "CInput.h"
#include "CCamera.h"
#include "CCutIn_PowerAttack.h"
class CSPGauge;
class CHPGauge;

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

	void TakeDamage(int damage) override;
private:
	// �ҋ@���
	void Update_Idle();
	// �ړ����
	void Update_Move();
	//�_�b�V���ړ����
	void Update_FastMove();
	// �U������
	void Update_Attack();
	// �U���҂�����
	void Update_AttackWait();
	// �U���I��
	void Update_AttackEnd();
	// �v���C���[�̌����Ă�������̃x�N�g�����擾
	void Update_AttackAngleVec(CVector* direction);
	// �ʏ�U��1-1����
	void Update_NormalAttack1();
	// �ʏ�U��1-2����
	void Update_NormalAttack2();
	// �ʏ�U��1-3����
	void Update_NormalAttack3();
	// �W�����v�U������
	void Update_AttackUp();
	// �󒆍U��1-1����
	void Update_AirAttack1();
	// �󒆍U��1-2����
	void Update_AirAttack2();
	// �󒆍U��1-3����
	void Update_AirAttack3();
	// �󒆍U��1-4����
	void Update_AirAttack4();
	// ���Z����
	void Update_SpecialMove();
	// �㓬�Z����
	void Update_Weak_SpecalMove();
	// �����Z����
	void Update_Strong_SpecalMove();
	// �����Z���̃v���C���[�̈ړ�����
	void Strong_SpecalMove_Move();
	// ��𓮍�
	void Update_Avoidance();

	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// ���[��Ԃ�؂�ւ���
	void SwitchDrawn();
	// ���[�̐؂�ւ�����
	void Update_SwitchDrawn();

	// ���Z�ɕK�v�ȏ���ݒ�
	// type : �Đ�����A�j���[�V�����@cutin : �J�b�g�C���̗L��
	void SetSpecalMove(EAnimType type, bool cutin);

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �v���C���[�̏��
	enum class EState
	{
		None = -1,
		eIdle,		// �ҋ@
		eMove,		// �ړ�
		eFastMove,	// �_�b�V���ړ�
		eAvoidance,	// ���
		eAttack,	// �U��
		eSpecalMove,// ���Z
	};
	EState mState;		// �v���C���[�̏��

	CVector mInput_save;// ���̓x�N�g�������ۑ�
	CVector mMoveSpeed;	// �ړ����x
	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsDrawn;		// �[����Ԃ�������Ԃ����� true:���� false:�[��
	bool mIsAirAttack;	// �󒆍U�����s�������ǂ���
	bool mIsCounter;	// �J�E���^�[�\�����ɍU���������������ǂ���
	bool mIsDash;		// �_�b�V�������ǂ���
	bool mSPZeroFlag;	// �X�^�~�i��0�ɂȂ������̃X�^�~�i�񕜒��̃t���O
	bool mIsUpdateInput;// ���͏����X�V����t���O

	int mAttackStep;	// �U���̒i�K

	float mHPRecoveryTime;	// HP�񕜒����p

	CColliderLine* mpColliderLine;
	CColliderSphere* mpBodyCol;

	CTransform* mpRideObject;
	CCutIn_PowerAttack* mpCutIn_PowerAttack;

	CHPGauge* mpHPGauge;	// HP�Q�[�W
	CSPGauge* mpSPGauge;	// SP�Q�[�W
};