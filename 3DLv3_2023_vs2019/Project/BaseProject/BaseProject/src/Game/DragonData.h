#pragma once
class DragonData
{
public:
	// �A�j���[�V�����̃f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����̃f�[�^�p�X
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
	// �A�j���[�V�����̃f�[�^�e�[�u��
	static std::vector<AnimData> ANIM_DATA;
};

// �A�j���[�V�����̎��
enum class EDragonAnimType
{
	None = -1,

	eIdle1,			// �ҋ@1
	eIdle2,			// �ҋ@2
	eLand,			// ���n
	eRun,			// ����
	eWalk,			// ����
	eScream,		// ���K
	eSleep,			// �Q��
	eTakeOff,		// ����
	eAttackFlame,	// �u���X�U��
	eAttackHand,	// ��ъ|����U��
	eAttackMouth,	// ���݂��U��
	eDie,			// ���S
	eFlyFlame,		// �󒆃u���X�U��
	eFlyForward,	// �󒆑O�i
	eFlyGlide,		// �󒆊���
	eFlyIdle,		// �󒆃A�C�h��
	eGetHit,		// �̂�����

	Num
};

#define GRAVITY			0.0625f	// �d��
#define ENEMY_HEIGHT	400.0f	// �G�̍���
#define FOV_LANGE		170.0f	// ���씻��p�̔��a
#define FOV_ANGLE		60.0f	// ���씻��p�̊p�x

// ���݂��U���֘A
// ���݂��U���̃R���C�_�[��؂�ւ���t���[��(�n��)
#define ATTACKMOUTH_COL_START 20.0f
// ���݂��U���̃R���C�_�[��؂�ւ���t���[��(�I���)
#define ATTACKMOUTH_COL_END 50.0f

// ��ъ|����U���֘A
// ��ъ|����U���̈ړ��J�n�t���[��
#define ATTACKHAND_START_FRAME 74.0f
// ��ъ|����U���̈ړ��I���t���[��
#define ATTACKHAND_END_FRAME 106.0f
// ��ъ|����U���̈ړ����x
#define ATTACKHAND_MOVESPEED 4.0f
// �o�b�N�X�e�b�v�̈ړ��J�n�t���[��(��ъ|����U���A�j���[�V������)
#define BACKSTEP_START_FRAME 148.0f
// �o�b�N�X�e�b�v�̈ړ��I���t���[��(��ъ|����U���A�j���[�V������)
#define BACKSTEP_END_FRAME 180.0f
// �o�b�N�X�e�b�v�̈ړ����x
#define BACKSTEP_MOVESPEED 3.9f
// �W�����v�n�߂̃t���[��
#define JUMP_START_FRAME 74.0f
// �o�b�N�X�e�b�v���̃W�����v�n�߂̃t���[��
#define BACKJUMP_START_FRAME 154.0f
// �������̒����t���[���P(�n��)
#define FALLING_FRAME1_START 84.0f
// �������̒����t���[��1(�I���)
#define FALLING_FRAME1_END 100.0f
// �������̒����t���[��2(�n��)
#define FALLING_FRAME2_START 170.0f
// �������̒����t���[��2(�I���)
#define FALLING_FRAME2_END 182.0f

// �u���X�U���֘A
// �u���X�U���J�n�t���[��
#define FLAME_START_FRAME 60.0f
// �u���X�U���I���t���[��
#define FLAME_END_FRAME 200.0f