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

	eIdle1,			// �ҋ@1(0.4)
	eIdle2,			// �ҋ@2
	eLand,			// ���n(0.325)
	eRun,			// ����(0.5)
	eWalk,			// ����(0.7)
	eScream,		// ���K(0.5)
	eTakeOff,		// ����(0.45)
	eAttackFlame,	// �u���X�U��(0.33)
	eAttackHand,	// ��ъ|����U��(0.5)
	eAttackMouth,	// ���݂��U��(0.43)
	eDie,			// ���S(0.25)
	eFlyFlame,		// �󒆃u���X�U��(0.5)
	eFlyForward,	// �󒆑O�i(0.6)
	eFlyGlide,		// �󒆊���
	eFlyIdle,		// �󒆃A�C�h��(0.31)
	eGetHit,		// �̂�����(0.49)
	eBackStep,		// �o�b�N�X�e�b�v(0.5)

	Num
};

#define GRAVITY			0.0625f	// �d��
#define ENEMY_HEIGHT	400.0f	// �G�̍���
#define FOV_LANGE		170.0f	// ���씻��p�̔��a
#define FOV_ANGLE		60.0f	// ���씻��p�̊p�x
#define FEAR_MAXVALUE	200.0f	// ���ݒl�̏��

// ���݂��U���֘A
// ���݂��U���̃R���C�_�[��؂�ւ���t���[��(�n��)
#define ATTACKMOUTH_COL_START 10.0f
// ���݂��U���̃R���C�_�[��؂�ւ���t���[��(�I���)
#define ATTACKMOUTH_COL_END 16.0f

// ��ъ|����U���֘A
// ��ъ|����U���̈ړ��J�n�t���[��
#define ATTACKHAND_START_FRAME 37.0f
// ��ъ|����U���̈ړ��I���t���[��
#define ATTACKHAND_END_FRAME 52.0f
// ��ъ|����U���̈ړ����x
#define ATTACKHAND_MOVESPEED 5.5f
// �o�b�N�X�e�b�v�̈ړ��J�n�t���[��(��ъ|����U���A�j���[�V������)
#define BACKSTEP_START_FRAME 77.0f
// �o�b�N�X�e�b�v�̈ړ��I���t���[��(��ъ|����U���A�j���[�V������)
#define BACKSTEP_END_FRAME 90.0f
// �o�b�N�X�e�b�v�̈ړ����x
#define BACKSTEP_MOVESPEED 5.0f
// �W�����v�n�߂̃t���[��
#define JUMP_START_FRAME 37.0f
// �o�b�N�X�e�b�v���̃W�����v�n�߂̃t���[��
#define BACKJUMP_START_FRAME 80.0f

// �������̒����t���[���P(�n��)
#define FALLING_FRAME1_START 42.0f
// �������̒����t���[��1(�I���)
#define FALLING_FRAME1_END 50.0f
// �������̒����t���[��2(�n��)
#define FALLING_FRAME2_START 85.0f
// �������̒����t���[��2(�I���)
#define FALLING_FRAME2_END 91.0f

// �o�b�N�X�e�b�v�̃��C�̒���
#define BACKSTEP_RAY 250.0f


// �u���X�U���֘A
// �u���X�U���J�n�t���[��
#define FLAME_START_FRAME 19.0f
// �u���X�U���I���t���[��
#define FLAME_END_FRAME 71.0f

// �󒆃u���X�U���֘A
// �󒆃u���X�J�n�t���[��
#define FLYFLAME_START_FRAME 15.0f
// �󒆃u���X�I���t���[��
#define FLYFLAME_END_FRAME 77.0f
// �c���HP����(50%)
#define HPPER_50 50
// �c���HP����(25%)
#define HPPER_25 25