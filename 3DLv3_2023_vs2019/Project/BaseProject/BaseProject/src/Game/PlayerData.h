#pragma once
class CPlayer;
class PlayerData
{
	friend CPlayer;
private:
	// �A�j���[�V�����̃f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];
};

// �v���C���[�̃A�j���[�V�����f�[�^�e�[�u��
const PlayerData::AnimData PlayerData::ANIM_DATA[] =
{
	{ "",													true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f	},	// �ҋ@(�[��)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f	},	// �ҋ@(����)
	{ "Character\\Player\\anim\\idleair.x",					true,	20.0f	},	// �󒆑ҋ@(�[��
	{ "Character\\Player\\anim\\idleair_combat.x",			true,	20.0f	},	// �󒆑ҋ@(����)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	101.0f	},	// �ҋ@����������
	{ "Character\\Player\\anim\\idle_sheathed_combat.x",	false,	81.0f	},	// �ҋ@���[������
	{ "Character\\Player\\anim\\run_start.x",				false,	36.0f	},	// ����J�n(�[��)
	{ "Character\\Player\\anim\\run_loop.x",				true,	40.0f	},	// ����(�[��)
	{ "Character\\Player\\anim\\run_end.x",					false,	50.0f	},	// ����I��(�[��)
	{ "Character\\Player\\anim\\run_combat_start.x",		false,	36.0f	},	// ����J�n(����)
	{ "Character\\Player\\anim\\run_combat.x",				true,	40.0f	},	// ����(����)
	{ "Character\\Player\\anim\\run_combat_end.x",			false,	50.0f	},	// ����I��(����)
	{ "Character\\Player\\anim\\run_drawn_combat.x",		false,	40.0f	},	// ���蒆��������
	{ "Character\\Player\\anim\\run_sheathed_combat.x",		false,	40.0f	},	// ���蒆�[������
	{ "Character\\Player\\anim\\fastrun_start.x",			false,	11.0f	},	// �_�b�V���J�n
	{ "Character\\Player\\anim\\fastrun_loop.x",			true,	28.0f	},	// �_�b�V��	
	{ "Character\\Player\\anim\\fastrun_end.x",				false,	52.0f	},	// �_�b�V���I��
	{ "Character\\Player\\anim\\roll_start.x",				false,	25.0f	},	// ��𓮍�J�n(�[��)
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	25.0f	},	// ��𓮍삩��A�C�h���ֈڍs(�[��)
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	20.0f	},	// ��𓮍삩�瑖��ֈڍs(�[��)
	{ "Character\\Player\\anim\\roll_combat.x",				false,	25.0f	},	// ��𓮍�J�n(����)
	{ "Character\\Player\\anim\\roll_end_combat_idle.x",	false,	20.0f	},	// ��𓮍삩��A�C�h���ֈڍs(����)
	{ "Character\\Player\\anim\\roll_end_combat_run.x",		false,	19.0f	},	// ��𓮍삩�瑖��ֈڍs(����)
	{ "Character\\Player\\anim\\run_attack.x",				false,	125.0f	},	// �����U��
	{ "Character\\Player\\anim\\attack_normal_1-1.x",		false,	24.0f	},	// �ʏ�U��1-1
	{ "Character\\Player\\anim\\attackwait_normal_1-1.x",	false,	39.0f	},	// �ʏ�U��1-1�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-1.x",	false,	30.0f	},	// �ʏ�U��1-1�I��
	{ "Character\\Player\\anim\\attack_normal_1-2.x",		false,	22.0f	},	// �ʏ�U��1-2
	{ "Character\\Player\\anim\\attackwait_normal_1-2.x",	false,	31.0f	},	// �ʏ�U��1-2�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-2.x",	false,	30.0f	},	// �ʏ�U��1-2�I��
	{ "Character\\Player\\anim\\attack_normal_1-3.x",		false,	56.0f	},	// �ʏ�U��1-3
	{ "Character\\Player\\anim\\attackwait_normal_1-3.x",	false,	28.0f	},	// �ʏ�U��1-3�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-3.x",	false,	30.0f	},	// �ʏ�U��1-3�I��
	{ "Character\\Player\\anim\\attack_air_1-1.x",			false,	11.0f	},	// �󒆍U��1-1
	{ "Character\\Player\\anim\\attackwait_air_1-1.x",		false,	13.0f	},	// �󒆍U��1-1�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-1.x",		false,	19.0f	},	// �󒆍U��1-1�I��
	{ "Character\\Player\\anim\\attack_air_1-2.x",			false,	14.0f	},	// �󒆍U��1-2
	{ "Character\\Player\\anim\\attackwait_air_1-2.x",		false,	16.0f	},	// �󒆍U��1-2�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-2.x",		false,	19.0f	},	// �󒆍U��1-2�I��
	{ "Character\\Player\\anim\\attack_air_1-3.x",			false,	16.0f	},	// �󒆍U��1-3
	{ "Character\\Player\\anim\\attackwait_air_1-3.x",		false,	14.0f	},	// �󒆍U��1-3�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-3.x",		false,	22.0f	},	// �󒆍U��1-3�I��
	{ "Character\\Player\\anim\\attack_air_1-4.x",			false,	20.0f	},	// �󒆍U��1-4
	{ "Character\\Player\\anim\\attackend_air_1-4.x",		false,	34.0f	},	// �ʏ�U��1-4�I��
	{ "Character\\Player\\anim\\attack_up.x",				false,	59.0f	},	// �W�����v�U��
	{ "Character\\Player\\anim\\landing.x",					false,	35.0f	},	// ���n����(�[��)
	{ "Character\\Player\\anim\\landing_combat.x",			false,	35.0f	},	// ���n����(����)
};
// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"

#define PLAYER_HEIGHT	1.8f
#define MOVE_SPEED		1.0f	// ���鑬�x
#define FASTMOVE_SPEED	1.5f	// �_�b�V�����x
#define ROLL_SPEED		1.3f	// ��𑬓x
#define JUMP_SPEED		1.5f
#define GRAVITY			0.0625f
#define JUMP_END_Y		1.0f
// �v���C���[������Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_DRAWN_IDLE_FRAME 35
// �v���C���[�[����Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// �v���C���[������Ԃ֐؂�ւ���t���[��(���蒆)
#define SWITCH_DRAWN_RUN_FRAME 13
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
#define ATTACK_UP_JUMP_SPEED 2.5f
