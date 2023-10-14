//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",												true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",				true,	601.0f	},	// �ҋ@
	{ "Character\\Player\\anim\\run_start.x",			false,	36.0f	},	// ����J�n
	{ "Character\\Player\\anim\\run_loop.x",			true,	40.0f	},	// ����
	{ "Character\\Player\\anim\\run_end.x",				false,	50.0f	},	// ����I��
	{ "Character\\Player\\anim\\fastrun_start.x",		false,	11.0f	},	// �_�b�V���J�n
	{ "Character\\Player\\anim\\fastrun_loop.x",		true,	28.0f	},	// �_�b�V��	
	{ "Character\\Player\\anim\\fastrun_end.x",			false,	52.0f	},	// �_�b�V���I��
	{ "Character\\Player\\anim\\roll.x",				false,	67.0f	},	// ��𓮍�	
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 1.0f			// ���鑬�x
#define FASTMOVE_SPEED 1.5f		//�_�b�V�����x
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mState_save(EState::None)
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

// �ҋ@
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	if (mIsGrounded)
	{
		// �ړ��L�[�������ꂽ������
		if (CInput::PushKey('W') || CInput::PushKey('A') ||
			CInput::PushKey('S') || CInput::PushKey('D'))
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
		else
		{
			// ����I���̃A�j���[�V�������I��������
			// �A�C�h���A�j���[�V�����ɐ؂�ւ���
			if (IsAnimationFinished())
			{
				ChangeAnimation(EAnimType::eIdle);
			}
		}

		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
	else
	{
		// �ҋ@��Ԃɐ؂�ւ�
		ChangeAnimation(EAnimType::eIdle);
	}
}

//�ړ�
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//�n�ʂɐڒn���Ă��邩����
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

			// ����o���A�j���[�V�������I��
			if (AnimationIndex()==
				(int)EAnimType::eRunStart && IsAnimationFinished())
			{
				// ����A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eRun);
			}
			//�_�b�V���ړ��̐؂�ւ�
			if (CInput::Key(VK_SHIFT))
			{
				mState = EState::eFastMove;
				ChangeAnimation(EAnimType::eFastRun);
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
		ChangeAnimation(EAnimType::eIdle);
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
			if (AnimationIndex() ==
				(int)EAnimType::eFastRunStart && IsAnimationFinished())
			{
				// �_�b�V���A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eFastRun);
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

// �U��
void CPlayer::Update_Attack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CPlayer::Update_AttackWait()
{
	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
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

// ��𓮍�
void CPlayer::Update_Avoidance()
{
	//�ۑ����Ă����Ԃ������l�̎��A���݂̏�Ԃ�ۑ�����
	if (mState_save == EState::None)
	{
		mState_save = mState;
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
		// �U��
		case EState::eAttack:
			Update_Attack();
			break;
		// �U���I���҂�
		case EState::eAttackWait:
			Update_AttackWait();
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
