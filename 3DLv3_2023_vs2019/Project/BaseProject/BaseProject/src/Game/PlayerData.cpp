#include "PlayerData.h"

// �v���C���[�̃A�j���[�V�����f�[�^�e�[�u��
std::vector<PlayerData::AnimData> PlayerData::ANIM_DATA = 
{
	/*				�@�p�X								�@ ���[�v �t���[��	���[�V�����l�@*/	
	{ "",													true,	0.0f,	0.0f },	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f,	0.0f },	// �ҋ@(�[��)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f,	0.0f },	// �ҋ@(����)
	{ "Character\\Player\\anim\\idleair.x",					true,	20.0f,	0.0f },	// �󒆑ҋ@(�[��
	{ "Character\\Player\\anim\\idleair_combat.x",			true,	20.0f,	0.0f },	// �󒆑ҋ@(����)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	101.0f,	0.0f },	// �ҋ@����������
	{ "Character\\Player\\anim\\idle_sheathed_combat.x",	false,	81.0f,	0.0f },	// �ҋ@���[������
	{ "Character\\Player\\anim\\run_start.x",				false,	36.0f,	0.0f },	// ����J�n(�[��)
	{ "Character\\Player\\anim\\run_loop.x",				true,	40.0f,	0.0f },	// ����(�[��)
	{ "Character\\Player\\anim\\run_end.x",					false,	50.0f,	0.0f },	// ����I��(�[��)
	{ "Character\\Player\\anim\\run_combat_start.x",		false,	36.0f,	0.0f },	// ����J�n(����)
	{ "Character\\Player\\anim\\run_combat.x",				true,	40.0f,	0.0f },	// ����(����)
	{ "Character\\Player\\anim\\run_combat_end.x",			false,	50.0f,	0.0f },	// ����I��(����)
	{ "Character\\Player\\anim\\run_drawn_combat.x",		false,	41.0f,	0.0f },	// ���蒆��������
	{ "Character\\Player\\anim\\run_sheathed_combat.x",		false,	40.0f,	0.0f },	// ���蒆�[������
	{ "Character\\Player\\anim\\fastrun_start.x",			false,	11.0f,	0.0f },	// �_�b�V���J�n
	{ "Character\\Player\\anim\\fastrun_loop.x",			true,	28.0f,	0.0f },	// �_�b�V��	
	{ "Character\\Player\\anim\\fastrun_end.x",				false,	52.0f,	0.0f },	// �_�b�V���I��
	{ "Character\\Player\\anim\\roll_start.x",				false,	26.0f,	0.0f },	// ��𓮍�J�n(�[��)
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	30.0f,	0.0f },	// ��𓮍삩��A�C�h���ֈڍs(�[��)
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	20.0f,	0.0f },	// ��𓮍삩�瑖��ֈڍs(�[��)
	{ "Character\\Player\\anim\\roll_combat.x",				false,	26.0f,	0.0f },	// ��𓮍�J�n(����)
	{ "Character\\Player\\anim\\roll_end_combat_idle.x",	false,	31.0f,	0.0f },	// ��𓮍삩��A�C�h���ֈڍs(����)
	{ "Character\\Player\\anim\\roll_end_combat_run.x",		false,	20.0f,	0.0f },	// ��𓮍삩�瑖��ֈڍs(����)
	{ "Character\\Player\\anim\\run_attack.x",				false,	125.0f,	1.0f },	// �����U��
	{ "Character\\Player\\anim\\attack_normal_1-1.x",		false,	24.0f,	1.1f },	// �ʏ�U��1-1
	{ "Character\\Player\\anim\\attackwait_normal_1-1.x",	false,	39.0f,	0.0f },	// �ʏ�U��1-1�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-1.x",	false,	30.0f,	0.0f },	// �ʏ�U��1-1�I��
	{ "Character\\Player\\anim\\attack_normal_1-2.x",		false,	22.0f,	1.2f },	// �ʏ�U��1-2
	{ "Character\\Player\\anim\\attackwait_normal_1-2.x",	false,	31.0f,	0.0f },	// �ʏ�U��1-2�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-2.x",	false,	30.0f,	0.0f },	// �ʏ�U��1-2�I��
	{ "Character\\Player\\anim\\attack_normal_1-3.x",		false,	56.0f,	1.4f },	// �ʏ�U��1-3
	{ "Character\\Player\\anim\\attackwait_normal_1-3.x",	false,	28.0f,	0.0f },	// �ʏ�U��1-3�U���҂�
	{ "Character\\Player\\anim\\attackend_normal_1-3.x",	false,	30.0f,	0.0f },	// �ʏ�U��1-3�I��
	{ "Character\\Player\\anim\\attack_air_1-1.x",			false,	11.0f,	1.3f },	// �󒆍U��1-1
	{ "Character\\Player\\anim\\attackwait_air_1-1.x",		false,	13.0f,	0.0f },	// �󒆍U��1-1�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-1.x",		false,	19.0f,	0.0f },	// �󒆍U��1-1�I��
	{ "Character\\Player\\anim\\attack_air_1-2.x",			false,	14.0f,	1.4f },	// �󒆍U��1-2
	{ "Character\\Player\\anim\\attackwait_air_1-2.x",		false,	16.0f,	0.0f },	// �󒆍U��1-2�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-2.x",		false,	19.0f,	0.0f },	// �󒆍U��1-2�I��
	{ "Character\\Player\\anim\\attack_air_1-3.x",			false,	16.0f,	1.5f },	// �󒆍U��1-3
	{ "Character\\Player\\anim\\attackwait_air_1-3.x",		false,	14.0f,	0.0f },	// �󒆍U��1-3�U���҂�
	{ "Character\\Player\\anim\\attackend_air_1-3.x",		false,	22.0f,	0.0f },	// �󒆍U��1-3�I��
	{ "Character\\Player\\anim\\attack_air_1-4.x",			false,	20.0f,	1.5f },	// �󒆍U��1-4
	{ "Character\\Player\\anim\\attackend_air_1-4.x",		false,	34.0f,	0.0f },	// �󒆍U��1-4�I��
	{ "Character\\Player\\anim\\attack_up.x",				false,	59.0f,	1.0f },	// �W�����v�U��
	{ "Character\\Player\\anim\\parry_counter_attack.x",	false,	81.0f,	5.0f },	// �J�E���^�[�U��
	{ "Character\\Player\\anim\\parry_counter_attack_end.x",false,	30.0f,	0.0f },	// �J�E���^�[�U���I��
	{ "Character\\Player\\anim\\attack_counter_start.x",	false,	25.0f,	0.0f },	// �J�E���^�[�\���J�n
	{ "Character\\Player\\anim\\attack_counter_loop.x",		true,	164.0f,	0.0f },	// �J�E���^�[�\����
	{ "Character\\Player\\anim\\attack_counter_end.x",		false,	46.0f,  0.0f },	// �J�E���^�[�\���I��
	{ "Character\\Player\\anim\\powerattack_start.x",		false,	20.0f,	0.0f },	// �����Z�\���J�n
	{ "Character\\Player\\anim\\powerattack_loop.x",		true,	45.0f,	0.0f },	// �����Z�\����
	{ "Character\\Player\\anim\\powerattack.x",				false,	139.0f,	8.0f },	// �����Z�U��
	{ "Character\\Player\\anim\\landing.x",					false,	35.0f,	0.0f },	// ���n����(�[��)
	{ "Character\\Player\\anim\\landing_combat.x",			false,	35.0f,	0.0f },	// ���n����(����)
	{ "Character\\Player\\anim\\die_start.x",				false,	61.0f,	0.0f },	// ���S�J�n(�[��)
	{ "Character\\Player\\anim\\die_loop.x",				true,	76.0f,	0.0f },	// ���S���[�v(�[��)
	{ "Character\\Player\\anim\\die_start_combat.x",		false,	61.0f,	0.0f },	// ���S�J�n(����)
	{ "Character\\Player\\anim\\die_loop_combat.x",			true,	76.0f,	0.0f },	// ���S���[�v(����)

};
