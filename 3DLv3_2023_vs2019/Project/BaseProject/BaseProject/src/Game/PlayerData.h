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
