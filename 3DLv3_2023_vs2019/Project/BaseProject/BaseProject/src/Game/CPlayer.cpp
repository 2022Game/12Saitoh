//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CDebugPrint.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",													true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f	},	// �ҋ@(�[��)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f	},	// �ҋ@(����)
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
	{ "Character\\Player\\anim\\roll_start.x",				false,	20.0f	},	// ��𓮍�J�n
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	20.0f	},	// ��𓮍삩��A�C�h���ֈڍs	
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	19.0f	},	// ��𓮍삩�瑖��ֈڍs
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
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 1.8f
#define MOVE_SPEED 1.0f			// ���鑬�x
#define FASTMOVE_SPEED 1.5f		// �_�b�V�����x
#define ROLL_SPEED 1.3f			// ��𑬓x
#define ATTACK_SPEED 0.5f		// �U�����̈ړ����x
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f
// �v���C���[������Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_DRAWN_IDLE_FRAME 35
// �v���C���[�[����Ԃ֐؂�ւ���t���[��(�ҋ@��Ԓ�)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// �v���C���[������Ԃ֐؂�ւ���t���[��(���蒆)
#define SWITCH_DRAWN_RUN_FRAME 13
// �v���C���[�[����Ԃ֐؂�ւ���t���[��(���蒆)
#define SWITCH_SHEATHED_RUN_FRAME 20

// �v���C���[�ʏ�U��1-1�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_1_START_FRAME	1
// �v���C���[�ʏ�U��1-1�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_1_END_FRAME 15
// �v���C���[�ʏ�U��1-1�̈ړ����x
#define NORMALATTACK1_1_MOVESPEED 0.5
// �v���C���[�ʏ�U��1-2�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_2_START_FRAME 0
// �v���C���[�ʏ�U��1-2�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_2_END_FRAME 12
// �v���C���[�ʏ�U��1-2�̈ړ����x
#define NORMALATTACK1_2_MOVESPEED 0.5
// �v���C���[�ʏ�U��1-3�̈ړ�����t���[�����(�n��)
#define NORMALATTACK1_3_START_FRAME 7
// �v���C���[�ʏ�U��1-3�̈ړ�����t���[�����(�I���)
#define NORMALATTACK1_3_END_FRAME 30
// �v���C���[�ʏ�U��1-3�̈ړ����x
#define NORMALATTACK1_3_MOVESPEED 0.5

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	//�����R���C�_�̐ݒ�
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

CPlayer::~CPlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	if (mpSword != nullptr)
	{
		delete mpSword;
		mpSword = nullptr;
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ���[��Ԃ�؂�ւ���
void CPlayer::SwitchDrawn()
{
	// ������Ԃ̎��A�[����Ԃ֐؂�ւ���
	if (mIsDrawn)
	{
		mIsDrawn = false;
	}
	else // �[����Ԃ̎��A������Ԃ֐؂�ւ���
	{
		mIsDrawn = true;
	}
}

// ���[�̐؂�ւ�����
void CPlayer::Update_SwitchDrawn()
{
	// ����̃A�j���[�V�����Ŕ��[��Ԃ�؂�ւ���
	// �A�j���[�V�������̃t���[���ɂ���Đ؂�ւ���^�C�~���O�����߂�
	switch (mIsDrawn)
	{
	case true:	// ����
		switch (mState)	// �v���C���[�̏��
		{
		case CPlayer::EState::eIdle: // �ҋ@���
			if (AnimationIndex() == (int)EAnimType::eIdle_Sheathed_Combat &&
				GetAnimationFrame() == SWITCH_SHEATHED_IDLE_FRAME){
				SwitchDrawn();
			}
			break;
		case CPlayer::EState::eMove: // �ړ����
			if (AnimationIndex() == (int)EAnimType::eRun_Sheathed_Combat &&
				GetAnimationFrame() == SWITCH_SHEATHED_RUN_FRAME){
				SwitchDrawn();
			}
		}
		break;
	case false:	// �[��
		switch (mState) // �v���C���[�̏��
		{
		case CPlayer::EState::eIdle: // �ҋ@���
			if (AnimationIndex() == (int)EAnimType::eIdle_Drawn_Combat &&
				GetAnimationFrame() == SWITCH_DRAWN_IDLE_FRAME) {
				SwitchDrawn();
			}
			break;
		case CPlayer::EState::eMove: // �ړ����
			if (AnimationIndex() == (int)EAnimType::eRun_Drawn_Combat) {
				if (GetAnimationFrame() == SWITCH_DRAWN_RUN_FRAME)
				SwitchDrawn();
			}
		}
		break;
	}
}

// �ҋ@
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	// ���[��Ԃ̔���
	switch (mIsDrawn)
	{
	case true:	// ������
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ��L�[�������ꂽ������
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// ����o���̃A�j���[�V�������Đ�
				ChangeAnimation(EAnimType::eRunStart_Combat);
				mState = EState::eMove;
			}
			else
			{
				// �A�j���[�V�������I��������
				// �A�C�h���A�j���[�V�����ɐ؂�ւ���
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle_Combat);
				}
			}
			// ���N���b�N�ōU����Ԃֈڍs
			if (CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eNormalAttack1_1);
			}
			// E�L�[�Ŕ[��
			if (CInput::PushKey('E'))
			{
				ChangeAnimation(EAnimType::eIdle_Sheathed_Combat);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	case false:	// �[����
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ��L�[�������ꂽ������
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// ����o���̃A�j���[�V�������Đ�
				ChangeAnimation(EAnimType::eRunStart);
				mState = EState::eMove;
				// �_�b�V���L�[��������Ă���ꍇ�_�b�V���ړ���Ԃɐ؂�ւ���
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRunStart);
				}
			}
			else // �ړ��L�[��������Ă��Ȃ���
			{
				// �A�j���[�V�������I��������
				// �A�C�h���A�j���[�V�����ɐ؂�ւ���
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle);
				}
				// ���N���b�N�Ŕ���
				if (CInput::PushKey(VK_LBUTTON))
				{
					// �����A�j���[�V�������Đ�
					ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
				}
			}
		}
		else
		{
			// �ЂƂ܂��́A�ҋ@���[�V������ǉ�
			// ��э~�蒆�̃��[�V������ǉ��\��
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

//�ړ�
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//���[��Ԃ̔���
	switch (mIsDrawn)
	{
	case true:	// ����
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ�����
			// �L�[�̓��̓x�N�g�����擾
			CVector input;
			if (CInput::Key('W'))		input.Z(1.0f);
			else if (CInput::Key('S'))	input.Z(-1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// ���̓x�N�g���̒����œ��͂���Ă��邩����
			if (input.LengthSqr() > 0)
			{
				// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// �A�j���[�V�������I��
				if (IsAnimationFinished())
				{
					// ����A�j���[�V�����ɐ؂�ւ�
					ChangeAnimation(EAnimType::eRun_Combat);
				}
				// ��𓮍�ւ̐؂�ւ�
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				// ���N���b�N�ōU����Ԃֈڍs
				if (CInput::PushKey(VK_LBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eNormalAttack1_1);
				}
				// E�L�[�Ŕ[��
				if (CInput::PushKey('E'))
				{
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				mState = EState::eIdle;
			}
		}
		else
		{
			/* �ЂƂ܂��́A�ҋ@���[�V������ǉ���э~�蒆�̃��[�V������ǉ��\��*/
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	case false:	// �[��
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ�����
			// �L�[�̓��̓x�N�g�����擾
			CVector input;
			if (CInput::Key('W'))		input.Z(1.0f);
			else if (CInput::Key('S'))	input.Z(-1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// ���̓x�N�g���̒����œ��͂���Ă��邩����
			if (input.LengthSqr() > 0)
			{
				// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// �A�j���[�V�������I��
				if (IsAnimationFinished())
				{
					// ����A�j���[�V�����ɐ؂�ւ�
					ChangeAnimation(EAnimType::eRun);
				}
				// �_�b�V���ړ��̐؂�ւ�
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRun);
				}
				// ��𓮍�ւ̐؂�ւ�
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				// ���N���b�N�Ŕ���
				if (CInput::PushKey(VK_LBUTTON))
				{
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd);
				mState = EState::eIdle;
			}
		}
		else
		{
			/* �ЂƂ܂��́A�ҋ@���[�V������ǉ���э~�蒆�̃��[�V������ǉ��\��*/
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

//�_�b�V���ړ�
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//�n�ʂɐڒn���Ă��邩����
	if (mIsGrounded)
	{
		// �ړ�����
		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		// �_�b�V���L�[�̓��͔���	true:���͒� false:����͒�
		bool dash = true;
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);
		if (CInput::PullKey(VK_SHIFT)) dash = false;

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0)
		{
			// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;

			// �_�b�V���J�n�A�j���[�V�������I��
			if (IsAnimationFinished())
			{
				// �_�b�V���A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eFastRun);
			}
			// ��𓮍�ւ̐؂�ւ�
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart);
			}
			// �_�b�V���L�[�𗣂����ꍇ
			// �_�b�V���̃A�j���[�V�������瑖��A�j���[�V�����ɐ؂�ւ���
			if (dash == false)
			{
				mState = EState::eMove;
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// �ړ��L�[��������Ă��Ȃ�
		else
		{
			// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
			ChangeAnimation(EAnimType::eFastRunEnd);
			mState = EState::eIdle;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle);
	}
}

// ��𓮍�
void CPlayer::Update_Avoidance()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �L�[�̓��̓x�N�g�����擾
	CVector input;

	if (CInput::Key('W'))		input.Z(1.0f);
	else if (CInput::Key('S'))	input.Z(-1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���ۑ��̓��̓x�N�g���������l�̏ꍇ
	if (mInput_save == CVector::zero)
	{
		// ���̓x�N�g���f�[�^���ꎞ�I�ɕۑ�
		mInput_save = input;
	}
	// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
	CVector move = CCamera::MainCamera()->Rotation() * mInput_save;
	move.Y(0.0f);
	move.Normalize();

	mMoveSpeed += move * ROLL_SPEED;

	// ��𓮍삪�I��������A�A�C�h����Ԃֈڍs����
	if (IsAnimationFinished())
	{
		// ��𓮍��Ɉړ��L�[���͂�����Α����Ԃֈڍs
		if (input.LengthSqr() > 0)
		{
			mState = EState::eMove;
			ChangeAnimation(EAnimType::eRollEnd_run);
			// �_�b�V���L�[��������Ă���ꍇ�A�_�b�V���ɐ؂�ւ���(�[�����̂�)
			if (CInput::Key(VK_SHIFT))
			{
				if (mIsDrawn == false)
				{
					mState = EState::eFastMove;
				}
			}
		}
		// �L�[���͂��Ȃ��ꍇ�̓A�C�h����Ԃֈڍs
		else
		{
			mState = EState::eIdle;
			ChangeAnimation(EAnimType::eRollEnd_idle);
		}
		// �ꎞ�I�ɕۑ��������̓x�N�g���������l�ɖ߂�
		mInput_save = CVector::zero;
	}
}

// �U��
void CPlayer::Update_Attack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �L�[�̓��̓x�N�g�����擾
	CVector input;
	if (CInput::Key('W'))		input.Z(1.0f);
	else if (CInput::Key('S'))	input.Z(-1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���ۑ��̓��̓x�N�g���������l�̏ꍇ
	if (mInput_save == CVector::zero)
	{
		// ���̓x�N�g���f�[�^���ꎞ�I�ɕۑ�
		mInput_save = input;
	}
	// �U���A�j���[�V�������I��������
	// �ꎞ�I�ɕۑ��������̓x�N�g��������������
	if (IsAnimationFinished())
	{
		mInput_save = CVector::zero;
	}
	// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
	CVector move = CCamera::MainCamera()->Rotation() * mInput_save;
	move.Y(0.0f);
	move.Normalize();

	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalAttack1_1:	// �ʏ�U��1-1����
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_1);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�������
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_2:	// �ʏ�U��1-2����
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_2);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�������
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_3:	// �ʏ�U��1-3����
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_3);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�������
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_3_MOVESPEED;
		}
	}

}

// �U���I���҂�
void CPlayer::Update_AttackWait()
{
	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalWait1_1:	// �ʏ�U��1-1�U���҂�
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-2�֐؂�ւ�
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_2);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}
		break;
	case (int)EAnimType::eNormalWait1_2:	// �ʏ�U��1-2�U���҂�
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-3�֐؂�ւ�
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_3);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		break;
	case (int)EAnimType::eNormalWait1_3:	// �ʏ�U��1-3�U���҂�
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-1�֐؂�ւ�
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_1);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
	}
	// �U���҂����[�V�����̊Ԃɉ������͂��Ȃ���΁A
	// �A�C�h����ԂɈڍs����
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
	}

	// �U���҂����[�V�������Ɉړ��{����L�[�̓��͂������
	// �����Ԃֈڍs����
	if (CInput::Key('W') || CInput::Key('A') ||
		CInput::Key('S') || CInput::Key('D'))
	{
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eAvoidance;
			ChangeAnimation(EAnimType::eRollStart);
		}
	}
}

// �U���I��
void CPlayer::Update_AttackEnd()
{
	// �ړ��L�[�����͂��ꂽ��ړ���Ԃֈڍs����
	if (CInput::Key('W') || CInput::Key('A') ||
		CInput::Key('S') || CInput::Key('D'))
	{
		mState = EState::eMove;
		ChangeAnimation(EAnimType::eRunStart_Combat);
		// ��𓮍�ւ̐؂�ւ�
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eAvoidance;
			ChangeAnimation(EAnimType::eRollStart);
		}
	}
	// �U���I�����[�V�������I��������ҋ@��Ԃֈڍs����
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �W�����v�J�n
void CPlayer::Update_JumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::Update_Jump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer::Update_JumpEnd()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			Update_Idle();
			break;
		// �ړ����
		case EState::eMove:
			Update_Move();
			break;
		// �_�b�V���ړ�
		case EState::eFastMove:
			Update_FastMove();
			break;
		// ������
		case EState::eAvoidance:
			Update_Avoidance();
			break;
		// �U��
		case EState::eAttack:
			Update_Attack();
			break;
		// �U���҂�
		case EState::eAttackWait:
			Update_AttackWait();
			break;
		// �U���I��
		case EState::eAttackEnd:
			Update_AttackEnd();
			break;
		// �W�����v�J�n
		case EState::eJumpStart:
			Update_JumpStart();
			break;
		// �W�����v��
		case EState::eJump:
			Update_Jump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			Update_JumpEnd();
			break;
	}

	// ����̏������N���A���Ă���ꍇ���[��؂�ւ���
	Update_SwitchDrawn();

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// �ړ�
	Position(Position() + mMoveSpeed);

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

#ifdef _DEBUG
	CVector pos = Position();
	CDebugPrint::Print("�v���C���[���:\n");
	CDebugPrint::Print("���W(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	CVector angles = EulerAngles();
	CDebugPrint::Print("��]�l(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	CDebugPrint::Print("�t���[�� : %.1f\n",GetAnimationFrame());
	if (mIsDrawn){
		CDebugPrint::Print("��� : ����");
	}
	else{
		CDebugPrint::Print("��� : �[��");
	}
#endif
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}

// �[������������������
bool CPlayer::IsDrawn()
{
	return mIsDrawn;
}