//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CDebugPrint.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mIsAirAttack(false)
	, mIsCounter(false)
	, mAttackStep(0)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = PlayerData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const PlayerData::AnimData& data = PlayerData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	// �X�e�[�^�X�̐ݒ�
	mStatas.atk = PLAYER_ATK;
	mStatas.def = PLAYER_DEF;
	mStatas.hp = PLAYER_HP;
	mStatas.sp = PLAYER_SP;
	mStatas.touki = PLAYER_TOUKI;

	//�����R���C�_�̐ݒ�
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	mpCutIn_PowerAttack = new CCutIn_PowerAttack();
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpModel);
	SAFE_DELETE(mpSword);

	mpCutIn_PowerAttack->Kill();
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	PlayerData::AnimData data = PlayerData::GetAnimData((int)type);
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ���[��Ԃ�؂�ւ���
void CPlayer::SwitchDrawn()
{
	// ������Ԃ̎��A�[����Ԃ֐؂�ւ���
	if (mIsDrawn){mIsDrawn = false;}
	// �[����Ԃ̎��A������Ԃ֐؂�ւ���
	else{mIsDrawn = true;}
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

// �X�V
void CPlayer::Update()
{
	if (mpCutIn_PowerAttack->IsPlaying())
	{
		// �L�����N�^�[�̍X�V
		CXCharacter::Update();
		return;
	}

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
		// ���Z
		case EState::eSpecalMove:
			Update_SpecialMove();
			break;
	}

	// ����̏������N���A���Ă���ꍇ���[��؂�ւ���
	Update_SwitchDrawn();

	// �d�͂�ݒ�
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// �ړ�
	Position(Position() + mMoveSpeed);

	if (mState != EState::eSpecalMove)
	{
		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = mMoveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}

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
	if (mIsDrawn) CDebugPrint::Print("����\n");
	else CDebugPrint::Print("�[��\n");
	
	CDebugPrint::Print("�v���C���[��� : ");
	if (mState == EState::eIdle)			CDebugPrint::Print("�ҋ@���\n");
	else if (mState == EState::eMove)		CDebugPrint::Print("�ړ����\n");
	else if (mState == EState::eFastMove)	CDebugPrint::Print("�_�b�V�����\n");
	else if (mState == EState::eAvoidance)	CDebugPrint::Print("������\n");
	else if (mState == EState::eAttack)		CDebugPrint::Print("�U�����\n");
	else if (mState == EState::eSpecalMove) CDebugPrint::Print("���Z���\n");

	CDebugPrint::Print("�U���� : %d\n", mStatas.atk);
	CDebugPrint::Print("�h��� : %d\n", mStatas.def);
	CDebugPrint::Print("HP : %d\n", mStatas.hp);
	CDebugPrint::Print("�X�^�~�i : %d\n", mStatas.sp);
	CDebugPrint::Print("���C�Q�[�W : %d\n", mStatas.touki);

	CDebugPrint::Print("�U���i�K : %d\n", mAttackStep);

	if (CInput::PushKey('Q'))
	{
		mIsCounter = true;
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
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
			mIsGrounded = true;
			

			// �[�����̋󒆑ҋ@��
			if (AnimationIndex() == (int)EAnimType::eIdleAir)
			{
				// ���n���Ɉړ��L�[�������Ă����ꍇ�A���n���[�V���������[�����O�ɐݒ�
				if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				else
				{
					// ���n����(�[��)���Đ�
					ChangeAnimation(EAnimType::eLanding);
				}
			}
			// �������̋󒆑ҋ@�܂��́A�󒆍U���I����
			else if (AnimationIndex() == (int)EAnimType::eIdleAir_Combat ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_1 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_1 ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_2 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_2 ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_3 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_3 ||
				AnimationIndex() == (int)EAnimType::eAirAttackEnd1_4)
			{
				// �ړ��L�[�������Ă����ꍇ�A���n���[�V���������[�����O�ɐݒ�
				if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
					mIsAirAttack = false;
				}
				else
				{
					// ���n����(����)���Đ�
					ChangeAnimation(EAnimType::eLandin_Combat);
					mIsAirAttack = false;
				}
			}

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