#pragma once
class PlayerData
{
public:
	// �A�j���[�V�����̃f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float motionValue;	// ���[�V�����l
	};
	// �A�j���[�V�����f�[�^�e�[�u�����擾
	static const AnimData& GetAnimData(int index)
	{
		return ANIM_DATA[index];
	}
	// �f�[�^�T�C�Y���擾
	static int GetAnimDataSize()
	{
		return ANIM_DATA.size();
	}
private:
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static std::vector<AnimData> ANIM_DATA;
};
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
	eRollStart,				// ��𓮍�J�n(�[��)
	eRollEnd_idle,			// �����A�C�h���ڍs(�[��)
	eRollEnd_run,			// ����㑖��ڍs(�[��)
	eRollStart_Combat,		// ��𓮍�J�n(����)
	eRollEnd_idle_Combat,	// �����A�C�h���ڍs(����)
	eRollEnd_run_Combat,	// ����㑖��ڍs(����)
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
	eAirAttack1_2,			// �󒆍U��1-2
	eAirAttackWait1_2,		// �󒆍U��1-2�U���҂�
	eAirAttackEnd1_2,		// �󒆍U��1-2�I��
	eAirAttack1_3,			// �󒆍U��1-3
	eAirAttackWait1_3,		// �󒆍U��1-3�U���҂�
	eAirAttackEnd1_3,		// �󒆍U��1-3�I��
	eAirAttack1_4,			// �󒆍U��1-4
	eAirAttackEnd1_4,		// �󒆍U��1-4�I��
	eAttack_Up,				// �W�����v�U��
	eParryAttack,			// �J�E���^�[�U��
	eParryAttack_End,		// �J�E���^�[�U���I��
	eCounter_Start,			// �J�E���^�[�\���J�n
	eCounter_Loop,			// �J�E���^�[�\����
	eCounter_End,			// �J�E���^�[�\���I��
	ePowerAttack_Start,		// �����Z�\���J�n
	ePowerAttack_Loop,		// �����Z�\����
	ePowerAttack,			// �����Z�U��
	eLanding,				// ���n����(�[��)
	eLandin_Combat,			// ���n����(����)
	eDie_Start,				// ���S�J�n(�[��)
	eDie_Loop,				// ���S���[�v(�[��)
	eDie_Start_Combat,		// ���S�J�n(����)
	eDie_Loop_Combat,		// ���S���[�v(����)
	Num
};

#define PLAYER_HEIGHT	1.8f	
#define MOVE_SPEED		1.0f	// ���鑬�x
#define FASTMOVE_SPEED	1.5f	// �_�b�V�����x
#define ROLL_SPEED		1.5f	// ��𑬓x
#define GRAVITY			0.0625f	// �d��

#define PLAYER_STATAS	0		// �v���C���[�f�[�^�e�[�u��
#define PLAYER_MAX_HP	100		// HP�̍ő�l
#define PLAYER_MIN_HP	0		// HP�̍ŏ��l
#define PLAYER_MAX_SP	100.0f	// �X�^�~�i�̍ő�l
#define PLAYER_MIN_SP	0.0f	// �X�^�~�i�̍ŏ��l
#define PLAYER_MAX_TOUKI 300	// ���C�Q�[�W�̍ő�l
#define PLAYER_MIN_TOUKI 0		// ���C�Q�[�W�̍ŏ��l
#define UP_TOUKI 10
#define ATTACKSTEP_END -1		// �U���I���̒i�K
#define ATTACKSTEP_COUNTER 3	// �J�E���^�[�̒i�K
#define AVOID_SPCOST 20.0f		// ����ɕK�v�ȃX�^�~�i

// �v���C���[������Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_DRAWN_IDLE_FRAME 35
// �v���C���[�[����Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// �v���C���[������Ԃ֐؂�ւ���t���[��(���蒆)
#define SWITCH_DRAWN_RUN_FRAME 15
// �v���C���[�[����Ԃ֐؂�ւ���t���[��(���蒆)
#define SWITCH_SHEATHED_RUN_FRAME 20

/* �ʏ�U�� */
// �v���C���[�ʏ�U��1-1�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_1_START_FRAME	1
// �v���C���[�ʏ�U��1-1�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_1_END_FRAME 15
// �v���C���[�ʏ�U��1-1�̈ړ����x
#define NORMALATTACK1_1_MOVESPEED 1.0f
// �v���C���[�ʏ�U��1-2�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_2_START_FRAME 1
// �v���C���[�ʏ�U��1-2�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_2_END_FRAME 12
// �v���C���[�ʏ�U��1-2�̈ړ����x
#define NORMALATTACK1_2_MOVESPEED 1.0f
// �v���C���[�ʏ�U��1-3�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_3_START_FRAME 7
// �v���C���[�ʏ�U��1-3�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_3_END_FRAME 30
// �v���C���[�ʏ�U��1-3�̈ړ����x
#define NORMALATTACK1_3_MOVESPEED 0.7f
// �v���C���[�ʏ�U��1-3�̃R���C�_�[�p�t���[��
#define NORMALATTACK1_3_COLLIDER 35

/* �W�����v�U�� */
// �W�����v�U���̈ړ�����t���[�����(�n��)
#define ATTACK_UP_START_FRAME 7
// �W�����v�U���̈ړ�����t���[�����(�I���)
#define ATTACK_UP_END_FRAME 19
// �W�����v�U���̈ړ����x
#define ATTACK_UP_MOVE_SPEED 1.0f
// �W�����v�U���̃W�����v���n�߂�t���[��
#define ATTACK_UP_JUMPSTART_FRAME 26
// �W�����v�U���̃W�����v�����x
#define ATTACK_UP_JUMP_SPEED 3.5f

/* �㓬�Z*/
// ��x�ڂ̍U���̊J�n�t���[��
#define COUNTERATTACK_START 25.0f

/* �����Z */
// ��x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK1_START 0
// ��x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK1_END 5
//�@��x�ڂ̈ړ������x�N�g��
#define POWERATTACK1_VEC CVector(0.0f, 0.0f, -1.0f)

// ��x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK2_START 8
// ��x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK2_END 12
//�@��x�ڂ̈ړ������x�N�g��
#define POWERATTACK2_VEC CVector(-1.0f, 0.0f, 1.0f)

// �O�x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK3_START 16
// �O�x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK3_END 20
// �O�x�ڂ̈ړ������x�N�g��
#define POWERATTACK3_VEC CVector(1.0f, 0.0f, 0.8f)

// �l�x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK4_START 24
// �l�x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK4_END 30
// �l�x�ڂ̈ړ������x�N�g��
#define POWERATTACK4_VEC CVector(-2.0f, 0.0f, 0.0f)

// �ܓx�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK5_START 36
// �ܓx�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK5_END 41
// �ܓx�ڂ̈ړ������x�N�g��
#define POWERATTACK5_VEC CVector(4.0f, 0.0f, -2.0f)

// �Z�x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK6_START 48
// �Z�x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK6_END 56
// �Z�x�ڂ̈ړ������x�N�g��
#define POWERATTACK6_VEC CVector(-1.0f, 0.1f, 1.0f)

// ���x�ڂ̈ړ�����t���[�����(�n��)
#define POWERATTACK7_START 58
// ���x�ڂ̈ړ�����t���[�����(�I���)
#define POWERATTACK7_END 65
// ���x�ڂ̈ړ������x�N�g��
#define POWERATTACL7_VEC CVector(0.0f, 3.0f, 0.0f)

// �����Z�̍U�����̈ړ����x
#define POWERATTACK_MOVESPEED 4.0f