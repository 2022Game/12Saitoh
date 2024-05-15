//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CDebugPrint.h"
#include "CHPGauge.h"
#include "CSPGauge.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CSword.h"

#include "CFlamethrower.h"
// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mMoveSpeed(CVector::zero)
	, mCamForward(CVector::zero)
	, mCamSide(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mIsAirAttack(false)
	, mIsCounter(false)
	, mIsDash(false)
	, mIsAvoid(false)
	, mSPZeroFlag(false)
	, mIsUpdateInput(false)
	, mStateStep(0)
	, mAttackStep(0)
	, mSPAttackStep(0)
	, mHPRecoveryTime(0.0f)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Player");

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

	//�����R���C�_�̐ݒ�
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(-0.5f,1.0f,0.0f),
		CVector(0.5f, 1.0f, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine3 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 1.0f, -0.5f),
		CVector(0.0f, 1.0f, 0.5f)
	);
	mpColliderLine3->SetCollisionLayers({ ELayer::eField });

	// �R���C�_�̐���
	// �����߂��p�R���C�_
	mpBodyCol = new CColliderSphere(this, ELayer::ePlayer, 0.5f);
	mpBodyCol->SetCollisionLayers({ ELayer::eEnemy });
	mpBodyCol->SetCollisionTags({ ETag::eEnemy });
	mpBodyCol->Position(0.0f, 1.0f, 0.0f);

	// �_���[�W�p�R���C�_
	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy, ETag::eFlame });
	mpDamageCol->Position(0.0f, 1.0f, 0.0f);

	// �J�b�g�C���J�����̐���
	mpCutIn_PowerAttack = new CCutIn_PowerAttack();

	// �v���C���[�̃X�e�[�^�X���擾
	mStatus = PLAYER_STATUS[PLAYER_STATAS];
	mMaxStatus = mStatus;
	mStatus.touki = 0;

	// HP�Q�[�W���쐬
	mpHPGauge = new CHPGauge();
	mpHPGauge->SetPos(10.0f, 10.0f);
	mpHPGauge->SetMaxValue(mStatus.hp);

	// SP�Q�[�W���쐬
	mpSPGauge = new CSPGauge();
	mpSPGauge->SetPos(10.0f, 40.0f);
	mpSPGauge->SetMaxValue(mStatus.sp);

	//�����쐬
	mpSword = new CSword();
	mpSword->Rotate(CVector(0.0f, 0.0f, 0.0f));

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f)
	);
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);
	SAFE_DELETE(mpColliderLine3);
	SAFE_DELETE(mpBodyCol);

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
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// �X�e�[�^�X�̐؂�ւ�
void CPlayer::ChangeState(EState state)
{
	mState = state;
	mStateStep = 0;
}

// ���[��Ԃ�؂�ւ���
void CPlayer::SwitchDrawn()
{
	// ������Ԃ̎��A�[����Ԃ֐؂�ւ���
	if (mIsDrawn) mIsDrawn = false;
	// �[����Ԃ̎��A������Ԃ֐؂�ւ���
	else mIsDrawn = true;
}

// ���[�̐؂�ւ�����
void CPlayer::Update_SwitchDrawn()
{
	switch (mStateStep)
	{
	case 0:
		EAnimType animType;
		float animFrame;
		// ����̃A�j���[�V�����Ŕ��[��Ԃ�؂�ւ���
		// �A�j���[�V�������̃t���[���ɂ���Đ؂�ւ���^�C�~���O�����߂�
		if (mIsDrawn)// ������
		{
			if (mState == EState::eIdle)// �A�C�h����
			{
				animType = EAnimType::eIdle_Sheathed_Combat;
				animFrame = SWITCH_SHEATHED_IDLE_FRAME;
			}
			else// ���蒆
			{
				animType = EAnimType::eRun_Sheathed_Combat;
				animFrame = SWITCH_SHEATHED_RUN_FRAME;
			}

			// �[������
			if (AnimationIndex() == (int)animType &&
				GetAnimationFrame() >= animFrame)
			{
				SwitchDrawn();
				mStateStep++;
			}
		}
		else// �[����
		{
			if (mState == EState::eIdle)// �A�C�h����
			{
				animType = EAnimType::eIdle_Drawn_Combat;
				animFrame = SWITCH_DRAWN_IDLE_FRAME;
			}
			else// ���蒆
			{
				animType = EAnimType::eRun_Drawn_Combat;
				animFrame = SWITCH_DRAWN_RUN_FRAME;
			}

			// ��������
			if (AnimationIndex() == (int)animType &&
				GetAnimationFrame() >= animFrame)
			{
				SwitchDrawn();
				mStateStep++;
			}
		}
		break;
	case 1:
		break;
	}
}

// �X�V
void CPlayer::Update()
{
	// �J�����̐��ʁE�T�C�h�x�N�g����ݒ�
	CCamera* mainCamera = CCamera::MainCamera();
	mCamForward = mainCamera->VectorZ();
	mCamSide = CVector::Cross(CVector::up, mCamForward);

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

	// �X�^�~�i�񕜏���
	// �_�b�V���܂��͉�𒆈ȊO�̎��͎��s
	if (!mIsDash && !mIsAvoid)
	{
		if (mStatus.sp <= PLAYER_MAX_SP)
		{
			mStatus.sp += 0.3;
		}
		if (mStatus.sp >= PLAYER_MAX_SP)
		{
			// SP���S��
			mSPZeroFlag = false;
		}
	}

	// �e�X�e�[�^�X�̏���l�Ɖ����l��ݒ�
	// ���
	if (mStatus.hp >= mMaxStatus.hp) mStatus.hp = mMaxStatus.hp; // HP
	if (mStatus.sp >= mMaxStatus.sp) mStatus.sp = mMaxStatus.sp; // SP
	if (mStatus.touki >= mMaxStatus.touki) mStatus.touki = mMaxStatus.touki;// ���C
	// ����
	if (mStatus.hp <= PLAYER_MIN_HP) mStatus.hp = PLAYER_MIN_HP; // HP
	if (mStatus.sp <= PLAYER_MIN_SP) mStatus.sp = PLAYER_MIN_SP; // SP
	if (mStatus.touki <= PLAYER_MIN_TOUKI) mStatus.touki = PLAYER_MIN_TOUKI;// ���C

	if (mpCutIn_PowerAttack->IsPlaying())
	{
		// �L�����N�^�[�̍X�V
		CXCharacter::Update();
		return;
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
	CDebugPrint::Print("FPS:%f\n", Time::FPS());
	CVector pos = Position();
	CDebugPrint::Print("�v���C���[���:\n");
	CDebugPrint::Print("���W(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	//CVector angles = EulerAngles();
	//CDebugPrint::Print("��]�l(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	//CDebugPrint::Print("�A�j���[�V�����t���[�� : %.1f\n",GetAnimationFrame());
	if (mIsDrawn) CDebugPrint::Print("����\n");
	else CDebugPrint::Print("�[��\n");
	
	CDebugPrint::Print("�v���C���[��� : ");
	if (mState == EState::eIdle)			CDebugPrint::Print("�ҋ@���\n");
	else if (mState == EState::eMove)		CDebugPrint::Print("�ړ����\n");
	else if (mState == EState::eFastMove)	CDebugPrint::Print("�_�b�V�����\n");
	else if (mState == EState::eAvoidance)	CDebugPrint::Print("������\n");
	else if (mState == EState::eAttack)		CDebugPrint::Print("�U�����\n");
	else if (mState == EState::eSpecalMove) CDebugPrint::Print("���Z���\n");

	CDebugPrint::Print("  HP �@: %d\n", mStatus.hp);
	CDebugPrint::Print("�U���� : %d\n", mStatus.atk);
	CDebugPrint::Print("�h��� : %d\n", mStatus.def);
	CDebugPrint::Print("�X�^�~�i : %.0f\n", mStatus.sp);
	CDebugPrint::Print("���C�Q�[�W : %d\n", mStatus.touki);

	CDebugPrint::Print("�U���i�K : %d\n", mAttackStep);
	
	//// �J�E���^�[�U���t���O�̕ύX
	//if (CInput::PushKey('Q')) mIsCounter = true;
	//
	//CDebugPrint::Print("�J�E���^�[�t���O : ");
	//if (mIsCounter) CDebugPrint::Print("ture\n");
	//else CDebugPrint::Print("false\n");

	//  1�L�[�������Ȃ���A�u���v�L�[��HP���� �u���v��HP����
	if (CInput::Key('1'))
	{
		if (CInput::Key(VK_UP)) mStatus.hp++;
		else if (CInput::Key(VK_DOWN)) mStatus.hp--;
	}
	// 2�L�[�������Ȃ���A�u���v�L�[��SP���� �u���v��SP����
	if (CInput::Key('2'))
	{
		if (CInput::Key(VK_UP)) mStatus.sp++;
		else if (CInput::Key(VK_DOWN)) mStatus.sp--;
	}

	//CDebugPrint::Print("���[�V�����l : %.2f\n", mMotionValue);

	//CDebugPrint::Print("�b��_���[�W : %d\n", mTemporaryDamage);
	//if (mTemporaryDamage > 0)
	//{
	//	if (mHPRecoveryTime >= 2)
	//	{
	//		mTemporaryDamage--;
	//		mStatus.hp++;
	//		mHPRecoveryTime = 0;
	//	}
	//	else {
	//		mHPRecoveryTime += 0.016666f;
	//	}
	//}

#endif
	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHPGauge->SetValue(mStatus.hp);
	// SP�Q�[�W�Ɍ��݂�SP��ݒ�
	mpSPGauge->SetValueF(mStatus.sp);
	mpSPGauge->SetSPZeroFlag(mSPZeroFlag);
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
					ChangeState(EState::eAvoidance);
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
					ChangeState(EState::eAvoidance);
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
	// �ǂƂ̏Փˏ���
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			Position(Position() + hit.adjust);
		}
	}

	// �G�Ƃ̏Փˏ���
	if (self == mpBodyCol)
	{
		Position(Position() +
			CVector(hit.adjust.X(), 0.0f, hit.adjust.Z()) * hit.weight);
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

// �_���[�W����
void CPlayer::TakeDamage(int damage)
{
	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		// ���S����
	}
	else
	{
		// �̂����鏈��
	}
}