#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderLine.h"
#include "CColliderTriangle.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CDebugPrint.h"
#include "CFlamethrower.h"
#include "CSPFlamethrower.h"
#include "Global.h"
#include "CHPGauge_Dragon.h"

CDragon* CDragon::spInstance = nullptr;

// �R���X�g���N�^
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mSaveVec(CVector::zero)
	, mRayAngleVec(CVector::zero)
	, mpHPGauge(nullptr)
	, mIsGrounded(true)
	, mIsAngry(false)
	, mChangeAngry(false)
	, mAngryStandardValue(0)
	, mAngryValue(0)
	, mFearValue(0)
	, mRandSave(0)
	, mBatteleStep(0)
	, mAttackStep(0)
	, mFearStep(0)
	, mSpAttackStep(0)
	, mSpAttackNum(0)
	, mElapsedTime(0.0f)
	, mAngryElapsedTime(0.0f)
	, mChaseElapsedTime(0.0f)
	, mFearElapsedTime(0.0f)
	, mAngle(0.0f)
	, mMotionBlurRemainTime(0.0f)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = DragonData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const DragonData::AnimData& data = DragonData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	ChangeAnimation(EDragonAnimType::eIdle1);
	SetAnimationSpeed(0.4f);

	// �����_���l�𐶐����Ď擾�����l�ɂ��X�e�[�^�X��ݒ�
	int rand = Math::Rand(0.0f, 5.0f);
	mStatus = ENEMY_STATUS[rand];
	mMaxStatus = mStatus;

	// �{��l�̍ő�l��ݒ�
	// �ő�l�͍ő�HP��2������Ƃ��Đݒ�
	mAngryStandardValue = mMaxStatus.hp * 0.2;

	// ��Ԃ̐ݒ�
	mState = EState::eIdle;

	// �R���C�_�[�̐���
	CreateCollider();

	// �Ή����˂̔��ˈʒu�̐ݒ�
	const CMatrix* flamemtx = GetFrameMtx("Armature_UpperMouth01");
	mpFlamethrower = new CFlamethrower
	(
		this, flamemtx,
		CVector(0.0f, -20.0f, 0.0f),
		CQuaternion(0.0f, 90.0f,0.0f).Matrix()
	);
	mpSpFlamethrower = new CSPFlamethrower
	(
		this, flamemtx,
		CVector(0.0f, -30.0f, 0.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);
}

// �f�X�g���N�^
CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);
	SAFE_DELETE(mpColliderLine3);
	// �_���[�W�p�R���C�_�[
	SAFE_DELETE(mpHeadDamageCol);
	SAFE_DELETE(mpNeckDamageCol);
	SAFE_DELETE(mpBodyDamageCol);
	SAFE_DELETE(mpTailDamageCol_Root);
	SAFE_DELETE(mpTailDamageCol_Tip);
	SAFE_DELETE(mpLegDamageCol_RF_Root);
	SAFE_DELETE(mpLegDamageCol_RF);
	SAFE_DELETE(mpLegDamageCol_RF_Tip);
	SAFE_DELETE(mpLegDamageCol_LF_Root);
	SAFE_DELETE(mpLegDamageCol_LF);
	SAFE_DELETE(mpLegDamageCol_LF_Tip);
	SAFE_DELETE(mpLegDamageCol_RB_Root);
	SAFE_DELETE(mpLegDamageCol_RB);
	SAFE_DELETE(mpLegDamageCol_RB_Tip);
	SAFE_DELETE(mpLegDamageCol_LB_Root);
	SAFE_DELETE(mpLegDamageCol_LB);
	SAFE_DELETE(mpLegDamageCol_LB_Tip);
	SAFE_DELETE(mpWingCol_R01);
	SAFE_DELETE(mpWingCol_R02);
	SAFE_DELETE(mpWingCol_R03);
	SAFE_DELETE(mpWingCol_R04);
	SAFE_DELETE(mpWingCol_R05);
	SAFE_DELETE(mpWingCol_R06);
	SAFE_DELETE(mpWingCol_L01);
	SAFE_DELETE(mpWingCol_L02);
	SAFE_DELETE(mpWingCol_L03);
	SAFE_DELETE(mpWingCol_L04);
	SAFE_DELETE(mpWingCol_L05);
	SAFE_DELETE(mpWingCol_L06);
	// �����߂��悤�R���C�_�[
	SAFE_DELETE(mpHeadCol);
	SAFE_DELETE(mpNeckCol);
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpTailCol_Root);
	SAFE_DELETE(mpTailCol_Tip);
	SAFE_DELETE(mpLegCol_RF_Root);
	SAFE_DELETE(mpLegCol_RF);
	SAFE_DELETE(mpLegCol_RF_Tip);
	SAFE_DELETE(mpLegCol_LF_Root);
	SAFE_DELETE(mpLegCol_LF);
	SAFE_DELETE(mpLegCol_LF_Tip);
	SAFE_DELETE(mpLegCol_RB_Root);
	SAFE_DELETE(mpLegCol_RB);
	SAFE_DELETE(mpLegCol_RB_Tip);
	SAFE_DELETE(mpLegCol_LB_Root);
	SAFE_DELETE(mpLegCol_LB);
	SAFE_DELETE(mpLegCol_LB_Tip);
	// �U���p�R���C�_�[
	SAFE_DELETE(mpAttackMouthCol);
	SAFE_DELETE(mpAttackHandCol);
	SAFE_DELETE(mpAttackScreamCol);
}

// �C���X�^���X���擾
CDragon* CDragon::Instance()
{
	return spInstance;
}

// �Q�[���J�n���ɃR���C�_�[�𐶐�
void CDragon::CreateCollider()
{
	/* �����R���C�_�[�̐ݒ� */
	// �ڒn����p
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	// �ǂƂ̓����蔻��p
	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(-150.0f, 200.0f, 0.0f),
		CVector(150.0f, 200.0f, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine3 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 200.0f, -300.0f),
		CVector(0.0f, 200.0f, 500.0f)
	);
	mpColliderLine3->SetCollisionLayers({ ELayer::eField });

	////* �_���[�W����p�R���C�_�[�̐ݒ� *////
	/* ���� */
	// ��
	mpHeadDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, -1.0f, 0.0f), CVector(-0.3f, 0.6f, 0.0f), 9.0f);
	mpHeadDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpHeadDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpHeadDamageCol->SetMeat(140);
	const CMatrix* headcol = GetFrameMtx("Armature_Tongue02");
	mpHeadDamageCol->SetAttachMtx(headcol);
	// ��
	mpNeckDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.3f, -0.9f, 0.0f), CVector(0.1f, 1.1f, 0.0f), 9.0f);
	mpNeckDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpNeckDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpNeckDamageCol->SetMeat(110);
	const CMatrix* neckcol = GetFrameMtx("Armature_Neck02");
	mpNeckDamageCol->SetAttachMtx(neckcol);

	/* ���� */
	// ��
	mpBodyDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, -1.0f, 0.0f), CVector(0.0f, 2.6f, 0.0f), 13.0f);
	mpBodyDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpBodyDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpBodyDamageCol->SetMeat(105);
	const CMatrix* bodycol = GetFrameMtx("Armature_Spine01");
	mpBodyDamageCol->SetAttachMtx(bodycol);
	// �K��(���{)
	mpTailDamageCol_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 1.8f, 0.0f), CVector(0.1f, -1.3f, 0.0f), 5.4f);
	mpTailDamageCol_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpTailDamageCol_Root->SetCollisionTags({ ETag::eWeapon });
	mpTailDamageCol_Root->SetMeat(105);
	const CMatrix* tailcol_root = GetFrameMtx("Armature_joint20");
	mpTailDamageCol_Root->SetAttachMtx(tailcol_root);
	// �K��(��[)
	mpTailDamageCol_Tip = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.2f, 3.0f, 0.0f), CVector(0.15f, -1.2f, 0.0f), 3.5f);
	mpTailDamageCol_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpTailDamageCol_Tip->SetCollisionTags({ ETag::eWeapon });
	mpTailDamageCol_Tip->SetMeat(100);
	const CMatrix* tailcol_tip = GetFrameMtx("Armature_joint22");
	mpTailDamageCol_Tip->SetAttachMtx(tailcol_tip);

	/* ��(���{) */
	// �E�O��
	mpLegDamageCol_RF_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, -0.1f), 6.0f);
	mpLegDamageCol_RF_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF_Root->SetMeat(105);
	const CMatrix* legcol_rf_root = GetFrameMtx("Armature_UpperArm_R");
	mpLegDamageCol_RF_Root->SetAttachMtx(legcol_rf_root);
	// ���O��
	mpLegDamageCol_LF_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.1f), 6.0f);
	mpLegDamageCol_LF_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF_Root->SetMeat(105);
	const CMatrix* legcol_lf_root = GetFrameMtx("Armature_UpperArm_L");
	mpLegDamageCol_LF_Root->SetAttachMtx(legcol_lf_root);
	// �E��둫
	mpLegDamageCol_RB_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegDamageCol_RB_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB_Root->SetMeat(95);
	const CMatrix* legcol_rb_root = GetFrameMtx("Armature_UpperReg_R");
	mpLegDamageCol_RB_Root->SetAttachMtx(legcol_rb_root);
	// ����둫
	mpLegDamageCol_LB_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegDamageCol_LB_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB_Root->SetMeat(95);
	const CMatrix* legcol_lb_root = GetFrameMtx("Armature_UpperLeg_L");
	mpLegDamageCol_LB_Root->SetAttachMtx(legcol_lb_root);

	/* �� */
	// �E�O��
	mpLegDamageCol_RF = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_RF->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF->SetMeat(105);
	const CMatrix* legcol_rf = GetFrameMtx("Armature_RowerArm_R");
	mpLegDamageCol_RF->SetAttachMtx(legcol_rf);
	// ���O��
	mpLegDamageCol_LF = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_LF->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF->SetMeat(105);
	const CMatrix* legcol_lf = GetFrameMtx("Armature_LowerArm_L");
	mpLegDamageCol_LF->SetAttachMtx(legcol_lf);
	// �E��둫
	mpLegDamageCol_RB = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_RB->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB->SetMeat(95);
	const CMatrix* legcol_rb = GetFrameMtx("Armature_RowerReg_R");
	mpLegDamageCol_RB->SetAttachMtx(legcol_rb);
	// ����둫
	mpLegDamageCol_LB = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_LB->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB->SetMeat(95);
	const CMatrix* legcol_lb = GetFrameMtx("Armature_LowerLeg_L");
	mpLegDamageCol_LB->SetAttachMtx(legcol_lb);

	/* ��(����) */
	// �E�O��
	mpLegDamageCol_RF_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_RF_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF_Tip->SetMeat(105);
	const CMatrix* legcol_rf_tip = GetFrameMtx("Armature_Middle01_R");
	mpLegDamageCol_RF_Tip->SetAttachMtx(legcol_rf_tip);
	// ���O��
	mpLegDamageCol_LF_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_LF_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF_Tip->SetMeat(105);
	const CMatrix* legcol_lf_tip = GetFrameMtx("Armature_Middle01_L");
	mpLegDamageCol_LF_Tip->SetAttachMtx(legcol_lf_tip);
	// �E��둫
	mpLegDamageCol_RB_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_RB_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB_Tip->SetMeat(95);
	const CMatrix* legcol_rb_tip = GetFrameMtx("Armature_MiddleToe01_R");
	mpLegDamageCol_RB_Tip->SetAttachMtx(legcol_rb_tip);
	// ����둫
	mpLegDamageCol_LB_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_LB_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB_Tip->SetMeat(95);
	const CMatrix* legcol_lb_tip = GetFrameMtx("Armature_MiddleToe01_L");
	mpLegDamageCol_LB_Tip->SetAttachMtx(legcol_lb_tip);

	/* �� */
	// �E��
	// R1
	mpWingCol_R01 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 2.2f, 0.0f));
	mpWingCol_R01->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R01->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R01->SetMeat(120);
	const CMatrix* wingcol_r01 = GetFrameMtx("Armature_WingIndex01_R");
	mpWingCol_R01->SetAttachMtx(wingcol_r01);
	// R2
	mpWingCol_R02 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 6.7f, 0.0f));
	mpWingCol_R02->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R02->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R02->SetMeat(120);
	const CMatrix* wingcol_r02 = GetFrameMtx("Armature_WingMiddle02_R");
	mpWingCol_R02->SetAttachMtx(wingcol_r02);
	// R3
	mpWingCol_R03 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 7.0f, 0.0f));
	mpWingCol_R03->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R03->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R03->SetMeat(120);
	const CMatrix* wingcol_r03 = GetFrameMtx("Armature_WingRing02_R");
	mpWingCol_R03->SetAttachMtx(wingcol_r03);
	// R4
	mpWingCol_R04 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 5.0f, 0.0f));
	mpWingCol_R04->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R04->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R04->SetMeat(120);
	const CMatrix* wingcol_r04 = GetFrameMtx("Armature_WingPinky02_R");
	mpWingCol_R04->SetAttachMtx(wingcol_r04);
	// R5
	mpWingCol_R05 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 4.3f, 0.0f));
	mpWingCol_R05->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R05->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R05->SetMeat(120);
	const CMatrix* wingcol_r05 = GetFrameMtx("Armature_Wing02_R");
	mpWingCol_R05->SetAttachMtx(wingcol_r05);
	// R6
	mpWingCol_R06 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 2.8f, 0.0f));
	mpWingCol_R06->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_R06->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_R06->SetMeat(120);
	const CMatrix* wingcol_r06 = GetFrameMtx("Armature_Wing01_R");
	mpWingCol_R06->SetAttachMtx(wingcol_r06);
	// ����
	// L1
	mpWingCol_L01 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 2.2f, 0.0f));
	mpWingCol_L01->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L01->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L01->SetMeat(120);
	const CMatrix* wingcol_l01 = GetFrameMtx("Armature_WingIndex01_L");
	mpWingCol_L01->SetAttachMtx(wingcol_l01);
	// L2
	mpWingCol_L02 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 6.7f, 0.0f));
	mpWingCol_L02->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L02->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L02->SetMeat(120);
	const CMatrix* wingcol_l02 = GetFrameMtx("Armature_WingMiddle02_L");
	mpWingCol_L02->SetAttachMtx(wingcol_l02);
	// L3
	mpWingCol_L03 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 7.0f, 0.0f));
	mpWingCol_L03->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L03->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L03->SetMeat(120);
	const CMatrix* wingcol_l03 = GetFrameMtx("Armature_WingRing02_L");
	mpWingCol_L03->SetAttachMtx(wingcol_l03);
	// L4
	mpWingCol_L04 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 5.0f, 0.0f));
	mpWingCol_L04->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L04->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L04->SetMeat(120);
	const CMatrix* wingcol_l04 = GetFrameMtx("Armature_WingPinky02_L");
	mpWingCol_L04->SetAttachMtx(wingcol_l04);
	// L5
	mpWingCol_L05 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 4.3f, 0.0f));
	mpWingCol_L05->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L05->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L05->SetMeat(120);
	const CMatrix* wingcol_l05 = GetFrameMtx("Armature_Wing02_L");
	mpWingCol_L05->SetAttachMtx(wingcol_l05);
	// L6
	mpWingCol_L06 = new CColliderLine(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 2.8f, 0.0f));
	mpWingCol_L06->SetCollisionLayers({ ELayer::eAttackCol });
	mpWingCol_L06->SetCollisionTags({ ETag::eWeapon });
	mpWingCol_L06->SetMeat(120);
	const CMatrix* wingcol_l06 = GetFrameMtx("Armature_Wing01_L");
	mpWingCol_L06->SetAttachMtx(wingcol_l06);

	////* �����߂��p�R���C�_�[�̐ݒ� *////
	/* ���� */
	// ��
	mpHeadCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, -1.0f, 0.0f), CVector(-0.3f, 0.6f, 0.0f), 9.0f);
	mpHeadCol->SetCollisionLayers({ ELayer::ePlayer });
	mpHeadCol->SetCollisionTags({ ETag::ePlayer });
	mpHeadCol->SetMeat(140);
	mpHeadCol->SetAttachMtx(headcol);
	// ��
	mpNeckCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.3f, -0.9f, 0.0f), CVector(0.1f, 1.1f, 0.0f), 9.0f);
	mpNeckCol->SetCollisionLayers({ ELayer::ePlayer });
	mpNeckCol->SetCollisionTags({ ETag::ePlayer });
	mpNeckCol->SetMeat(110);
	mpNeckCol->SetAttachMtx(neckcol);

	/* ���� */
	// ��
	mpBodyCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, -1.0f, 0.0f), CVector(0.0f, 2.6f, 0.0f), 13.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });
	mpBodyCol->SetMeat(105);
	mpBodyCol->SetAttachMtx(bodycol);
	// �K��(���{)
	mpTailCol_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 1.8f, 0.0f), CVector(0.1f, -1.3f, 0.0f), 5.4f);
	mpTailCol_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpTailCol_Root->SetCollisionTags({ ETag::ePlayer });
	mpTailCol_Root->SetMeat(105);
	mpTailCol_Root->SetAttachMtx(tailcol_root);
	// �K��(��[)
	mpTailCol_Tip = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.2f, 3.0f, 0.0f), CVector(0.15f, -1.2f, 0.0f), 3.5f);
	mpTailCol_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpTailCol_Tip->SetCollisionTags({ ETag::ePlayer });
	mpTailCol_Tip->SetMeat(100);
	mpTailCol_Tip->SetAttachMtx(tailcol_tip);

	/* ��(���{) */
	// �E�O��
	mpLegCol_RF_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, -0.1f), 6.0f);
	mpLegCol_RF_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF_Root->SetMeat(105);
	mpLegCol_RF_Root->SetAttachMtx(legcol_rf_root);
	// ���O��
	mpLegCol_LF_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.1f), 6.0f);
	mpLegCol_LF_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF_Root->SetMeat(105);
	mpLegCol_LF_Root->SetAttachMtx(legcol_lf_root);
	// �E��둫
	mpLegCol_RB_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegCol_RB_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB_Root->SetMeat(95);
	mpLegCol_RB_Root->SetAttachMtx(legcol_rb_root);
	// ����둫
	mpLegCol_LB_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegCol_LB_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB_Root->SetMeat(95);
	mpLegCol_LB_Root->SetAttachMtx(legcol_lb_root);

	/* �� */
	// �E�O��
	mpLegCol_RF = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_RF->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF->SetMeat(105);
	mpLegCol_RF->SetAttachMtx(legcol_rf);
	// ���O��
	mpLegCol_LF = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_LF->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF->SetMeat(105);
	mpLegCol_LF->SetAttachMtx(legcol_lf);
	// �E��둫
	mpLegCol_RB = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_RB->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB->SetMeat(95);
	mpLegCol_RB->SetAttachMtx(legcol_rb);
	// ����둫
	mpLegCol_LB = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_LB->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB->SetMeat(95);
	mpLegCol_LB->SetAttachMtx(legcol_lb);

	/* ��(����) */
	// �E�O��
	mpLegCol_RF_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_RF_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF_Tip->SetMeat(105);
	mpLegCol_RF_Tip->SetAttachMtx(legcol_rf_tip);
	// ���O��
	mpLegCol_LF_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_LF_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF_Tip->SetMeat(105);
	mpLegCol_LF_Tip->SetAttachMtx(legcol_lf_tip);
	// �E��둫
	mpLegCol_RB_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_RB_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB_Tip->SetMeat(95);
	mpLegCol_RB_Tip->SetAttachMtx(legcol_rb_tip);
	// ����둫
	mpLegCol_LB_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_LB_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB_Tip->SetMeat(95);
	mpLegCol_LB_Tip->SetAttachMtx(legcol_lb_tip);

	////* �U���p�R���C�_�[�̐ݒ� *////
	// ���݂��U��
	mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	const CMatrix* mouthcol = GetFrameMtx("Armature_Tongue02");
	mpAttackMouthCol->SetAttachMtx(mouthcol);
	// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	mpAttackMouthCol->SetEnable(false);
	// ��ъ|����U��
	mpAttackHandCol = new CColliderSphere(this, ELayer::eAttackCol, 4.0f);
	mpAttackHandCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackHandCol->SetCollisionTags({ ETag::ePlayer });
	const CMatrix* handcol = GetFrameMtx("Armature_Spine02");
	mpAttackHandCol->SetAttachMtx(handcol);
	// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	mpAttackHandCol->SetEnable(false);
	// ���K�U��
	mpAttackScreamCol = new CColliderSphere(this, ELayer::eAttackCol, 7.0f);
	mpAttackScreamCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackScreamCol->SetCollisionTags({ ETag::ePlayer });
	const CMatrix* screamcol = GetFrameMtx("Armature_Spine01");
	mpAttackScreamCol->SetAttachMtx(screamcol);
	// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	mpAttackScreamCol->SetEnable(false);
}

// �R���C�_�[�̍X�V����
void CDragon::ColliderUpdate()
{
	////* �_���[�W�p�R���C�_�[ *////
	/* ���� */
	mpHeadDamageCol->Update();	// ��
	mpNeckDamageCol->Update();	// ��
	/* ���� */
	mpBodyDamageCol->Update();		// ����
	mpTailDamageCol_Root->Update();// �K��(���{)
	mpTailDamageCol_Tip->Update(); // �K��(��[)
	/* �� */
	// �E�O��
	mpLegDamageCol_RF_Root->Update();
	mpLegDamageCol_RF->Update();
	mpLegDamageCol_RF_Tip->Update();
	// ���O��
	mpLegDamageCol_LF_Root->Update();
	mpLegDamageCol_LF->Update();
	mpLegDamageCol_LF_Tip->Update();
	// �E��둫
	mpLegDamageCol_RB_Root->Update();
	mpLegDamageCol_RB->Update();
	mpLegDamageCol_RB_Tip->Update();
	// ����둫
	mpLegDamageCol_LB_Root->Update();
	mpLegDamageCol_LB->Update();
	mpLegDamageCol_LB_Tip->Update();
	/* �� */
	// �E��
	mpWingCol_R01->Update();
	mpWingCol_R02->Update();
	mpWingCol_R03->Update();
	mpWingCol_R04->Update();
	mpWingCol_R05->Update();
	mpWingCol_R06->Update();
	// ����
	mpWingCol_L01->Update();
	mpWingCol_L02->Update();
	mpWingCol_L03->Update();
	mpWingCol_L04->Update();
	mpWingCol_L05->Update();
	mpWingCol_L06->Update();

	////* �����߂��p�R���C�_�[ *////
	 /* ���� */
	mpHeadCol->Update();
	mpNeckCol->Update();
	/* ���� */
	mpBodyCol->Update();
	mpTailCol_Root->Update();
	mpTailCol_Tip->Update();
	/* �� */
	// �E�O��
	mpLegCol_RF_Root->Update();
	mpLegCol_RF->Update();
	mpLegCol_RF_Tip->Update();
	// ���O��
	mpLegCol_LF_Root->Update();
	mpLegCol_LF->Update();
	mpLegCol_LF_Tip->Update();
	// �E��둫
	mpLegCol_RB_Root->Update();
	mpLegCol_RB->Update();
	mpLegCol_RB_Tip->Update();
	// ����둫
	mpLegCol_LB_Root->Update();
	mpLegCol_LB->Update();
	mpLegCol_LB_Tip->Update();

	////* �U���p�R���C�_�[ *////
	mpAttackMouthCol->Update();
	mpAttackHandCol->Update();
}

// �A�j���[�V�����̐؂�ւ�
void CDragon::ChangeAnimation(EDragonAnimType type)
{
	if (!(EDragonAnimType::None < type && type < EDragonAnimType::Num)) return;
	DragonData::AnimData data = DragonData::GetAnimData((int)type);
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
	SetAnimationSpeed(1.0f);
}

// �{���Ԃ̐؂�ւ�����
void CDragon::ChangeAngry()
{
	// �{��l����l�ɍ��킹��
	mAngryValue = mAngryStandardValue;
	mIsAngry = true;
	// �{���Ԃւ̈ڍs���ə��K�U�����s��
	ChangeAnimation(EDragonAnimType::eScream);
	SetAnimationSpeed(0.5f);
	// �e�X�e�[�^�X������
	mStatus.atk += 10;
	mStatus.def += 10;
	// �{���Ԃւ̈ڍs�t���O�����ɖ߂�
	mChangeAngry = false;

	// UpdateBattle_Idle()���Ŏg�p�Ă���
	// �ϐ������������Ă���
	mElapsedTime = 0.0f;
	mRandSave = 0;
	// �퓬�̒i�K�͍U�������ɂ���
	mBatteleStep = 2;
}

// ���[�V�����u���[�̍X�V����
void CDragon::UpdateMotionBlur()
{
	// ���[�V�����u���[�̎c�莞�Ԃ��c���Ă��Ȃ���΁A�������Ȃ�
	if (mMotionBlurRemainTime <= 0.0f) return;
	// ���݂̃J�������擾���A���݂��Ȃ���Ώ������Ȃ�
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// �J�����̌����Ɣ��Ε����փu���[���|���邽�߁A
	// ���]�����J�����̌������擾
	
	CVector pPos = CPlayer::Instance()->Position();
	CVector dPos = Position();
	CVector DP = (dPos - pPos).Normalized();
	CVector camDir = DP;

	// �c�莞�Ԃ���o�ߎ��Ԃ̊������擾�i�o�ߎ��Ԃ̊��� = 1 - �c�莞�Ԃ̊����j
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// �u���[�̕����T�C���J�[�u�Ōo�ߎ��Ԃɍ��킹�đ���������
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// ���[�V�����u���[�̃p�����[�^��ݒ�
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// �c�莞�Ԃ��o�ߎ��ԕ�����������
	mMotionBlurRemainTime -= Time::DeltaTime();
	// �c�莞�Ԃ��Ȃ��Ȃ�΁A
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// ���[�V�����u���[���I�t�ɂ���
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

// �v���C���[�����������ǂ���
bool CDragon::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// ����p�x�̔���
	// ���g����v���C���[�܂ł̃x�N�g�����擾
	CVector EP = (playerPos - enemyPos).Normalized();
	// ���g�̐��ʕ����̃x�N�g�����擾
	CVector forward = VectorZ().Normalized();
	// ���ʕ����̃x�N�g���ƃv���C���[�܂ł�
	// ���ς���p�x�����߂�
	float dotZ = forward.Dot(EP);

	// ���߂��p�x������p�x�O�̏ꍇ�Afalse��Ԃ�
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	// �����̔���
	// ���g����v���C���[�܂ł̋��������߂�
	float distance = (playerPos - enemyPos).Length();
	// ���߂����������싗�����������ꍇ�Afalse��Ԃ�
	if (distance > FOV_LANGE) return false;

	// ���씻��Ƌ��������ʂ����̂�true��Ԃ�
	return true;
}

// �o�b�N�X�e�b�v�ł��邩�ǂ���
bool CDragon::IsBackStep() const
{
	// ���C�̊J�n�n�_
	CVector startPos = Position() + CVector(0.0f, 50.0f, 0.0f);
	// ���C�̏I���n�_
	CVector endPos = Position() + -VectorZ().Normalized() * BACKSTEP_RAY + CVector(0.0f, 50.0f, 0.0f);
	// �Փˈʒu�܂ł̋����ԋp�p
	float outDist = 0.0f;

	// �t�B�[���h�ƃ��C�̓����蔻��s��
	if (gField->CollisionRay(startPos, endPos, &outDist))
	{
		return false;
	};

	// ���ɏ\���ȃX�y�[�X������̂�
	// �o�b�N�X�e�b�v���\
	return true;
}

// ���C���΂��Ĉړ��ł���p�x���擾
CVector CDragon::GetRayAngleVec()
{
	CVector dPos = Position();
	CVector pPos = CPlayer::Instance()->Position();
	CVector PD = pPos - dPos;
	PD.Y(0.0f);
	PD.Normalize();

	float rayLength = FIELD_RADIUS * 1.5;

	// ���C�̊J�n�n�_
	CVector startPos = Position();
	startPos.Y(100.0f);
	// ���C�̏I���n�_
	CVector endPos = startPos + PD * rayLength;
	// �Փˈʒu�܂ł̋����ԋp�p
	float outDist = 0.0f;
	float angle = 0.0f;

	float addAngle = 15.0f;
	if (Math::Rand(0, 1) == 0)
	{
		addAngle *= -1.0f;
	}
	
#ifdef _DEBUG
	mRayAngleData.clear();

	CRayAngleData data;
	data.start = startPos;
	data.end = endPos;
	data.dist = rayLength;
	mRayAngleData.push_back(data);

	int index = 0;
#endif
	// �t�B�[���h�ƃ��C�̓����蔻����s��
	while (gField->CollisionRay(startPos, endPos, &outDist))
	{
#ifdef _DEBUG
		mRayAngleData[index].dist = outDist;
#endif

		angle += addAngle;

		CVector v = CQuaternion(0.0f, angle, 0.0f) * PD;
		endPos = startPos + v * rayLength;

#ifdef _DEBUG
		data.start = startPos;
		data.end = endPos;
		data.dist = rayLength;
		mRayAngleData.push_back(data);

		index++;
#endif
	}

	return (endPos - startPos).Normalized();
}

//�v���C���[�Ƃ̋������擾
CDragon::EDistanceType CDragon::PlayerFromDistance()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// �����̔���
	float distance = (playerPos - enemyPos).Length();
	// �����Ɣ��ʗp�̒l���ׂāA�����𔻕�
	if (distance >= 280.0f) mDistanceType = EDistanceType::eFar;// ������
	else if (distance >= 230.0f) mDistanceType = EDistanceType::eMedium;// ������
	else mDistanceType = EDistanceType::eNear;// �ߋ���

	// ���ʂ���������Ԃ�
	return mDistanceType;
}

// �X�V����
void CDragon::Update()
{
	// ��Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �퓬���
	case EState::eBattle:
		UpdateBattle();
		break;
		// �K�E�Z�U��
	case EState::eSpAttack:
		UpdateSpecalAttack();
		break;
		// ���S���
	case EState::eDeath:
		UpdateDie();
		break;
	}

	// �d�͂̐ݒ�
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// �ړ�
	Position(Position() + mMoveSpeed);

	// �U�����܂��́A�o�b�N�X�e�b�v�ȊO�̎��͈ړ������֌�����
	if (mBatteleStep != 2 &&
		AnimationIndex() != (int)EDragonAnimType::eBackStep)
	{
		CVector current = VectorZ();
		CVector target = mMoveSpeed;
		target.Y(0.0f);
		target.Normalize();
		if (target.LengthSqr() > 0.0f)
		{
			float t = 0.02f;
			// �󒆈ړ��̎��̂ݕ⊮�����𒲐�
			if (AnimationIndex() == (int)EDragonAnimType::eFlyForward)
			{
				t = 0.2f;
			}

			CVector forward = CVector::Slerp(current, target, t);
			Rotation(CQuaternion::LookRotation(forward));
		}
	}
	CXCharacter::Update();
	// �e�R���C�_�[�̍X�V����
	ColliderUpdate();
	// ���[�V�����u���[�̍X�V����
	UpdateMotionBlur();
#ifdef _DEBUG
	// �h���S���̃X�e�[�^�X��\��
	CDebugPrint::Print("�G�X�e�[�^�X\n");
	CDebugPrint::Print("���x�� : %d\nHP : %d\n�U���� : %d\n�h��� : %d\n",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);
	// �{���Ԃ��ǂ���
	mIsAngry ? CDebugPrint::Print("�{����\n") : CDebugPrint::Print("��{����\n");
	// �_���[�W��\��
	CDebugPrint::Print("%d\n", mDamage);
	//// �v���C���[�Ƃ̋�����\��
	//CVector pPos = CPlayer::Instance()->Position();
	//CVector ePos = Position();
	//float distance = (pPos - ePos).Length();
	//CDebugPrint::Print("�v���C���[�Ƃ̋��� : %.1f\n", distance);

	//CDebugPrint::Print("�v���C���[�Ƃ̋���\n");
	//if (mDistanceType == EDistanceType::eNear) CDebugPrint::Print("�ߋ���\n");
	//else if (mDistanceType == EDistanceType::eMedium) CDebugPrint::Print("������\n");
	//else CDebugPrint::Print("������\n");

	//if (CInput::PushKey('1'))
	//{
	//	int p = GetHPPercent();
	//	printf("%d\n", p);
	//}

	// �o�b�N�X�e�b�v����p
	//if (IsBackStep()) CDebugPrint::Print("true\n");
	//else CDebugPrint::Print("false\n");
	if (CInput::Key('N'))
	{
		mStatus.hp-= 50000;
	}

#endif
	if (mpHPGauge != nullptr) mpHPGauge->SetValue(mStatus.hp);
}

// ���S����
void CDragon::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
}

// �Փˏ���
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �n�ʂƂ̓����蔻��
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
			mIsGrounded = true;
		}
	}
	// �ǂƂ̓����蔻��
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
		}
	}

	// �Փ˂����R���C�_�[���U������p�̃R���C�_�[�ł����
	if (self == mpAttackMouthCol ||
		self == mpAttackHandCol  ||
		self == mpAttackScreamCol)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł��邩��
		// ���G��ԂŖ������
		CPlayer* player = CPlayer::Instance();
		if (chara != nullptr && !player->IsInvincible())
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				// �_���[�W�v�Z
				int atk = Status().atk;
				int def = player->Status().def;
				float motionvalue = GetMotionValue();
				// �_���[�W�v�Z
				int damage = player->TakePlayerToDamage(atk, def, motionvalue);

				// �_���[�W��^����
				chara->TakeDamage(damage);

				// �U���ς݂̃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
}

// �`�揈��
void CDragon::Render()
{
	CXCharacter::Render();
#ifdef _DEBUG
	// ���씻��p
	//Primitive::DrawSector(
	//	Position() + CVector(0.0f, 1.0, 0.0f),
	//	-EulerAngles(),
	//	-FOV_ANGLE,
	//	FOV_ANGLE,
	//	FOV_LANGE,
	//	CColor::red,
	//	45
	//);

	// �t�B�[���h�Ƃ̃��C���茋�ʂ�\��
	int size = mRayAngleData.size();
	for (int i = 0; i < size; i++)
	{
		CRayAngleData data = mRayAngleData[i];

		// ���C���ǂƏՓ˂����f�[�^
		if (i < size - 1)
		{
			CVector vec = (data.end - data.start).Normalized();
			CVector pos = data.start + vec * data.dist;
			// �ǂɏՓ˂����O�܂ł̃x�N�g����\��
			Primitive::DrawLine
			(
				data.start, pos,
				i == 0 ? CColor::red : CColor::yellow,
				3.0f
			);
			// �ǂɏՓ˂�����̃x�N�g����\��
			Primitive::DrawLine
			(
				pos, data.end,
				CColor::blue,
				3.0f
			);
		}
		// ���C���Փ˂��Ȃ������f�[�^
		else
		{
			Primitive::DrawLine
			(
				data.start, data.end,
				CColor::green,
				3.0f
			);
		}
	}
#endif
}

// �_���[�W����
void CDragon::TakeDamage(int damage)
{
	mDamage = damage;
	// �_���[�W��HP������
	mStatus.hp -= damage;
	// �_���[�W�̈ꊄ�������ݒl�ɉ��Z
	mFearValue += damage / 10;

	// �{���ԂŖ�����΁A�_���[�W���{��l����
	if (!mIsAngry) mAngryValue += damage;
	// �{���ԂŗL��΁A�_���[�W�̔����̒l�����{��l������
	else mAngryValue -= damage / 2;

	// HP��0�ɂȂ����玀�S�������s��
	if (mStatus.hp <= 0)
	{
		// ���S����
		mState = EState::eDeath;
		ChangeAnimation(EDragonAnimType::eDie);
		SetAnimationSpeed(0.25);
	}
}

// �U���J�n
void CDragon::AttackStart()
{
	CCharaBase::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eAttackMouth: // ���݂��U��
		mpAttackMouthCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eAttackHand: // ��ъ|����U��
		mpAttackHandCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eScream: // ���K�U��
		mpAttackScreamCol->SetEnable(true);
		break;
	}
}

// �U���I��
void CDragon::AttackEnd()
{
	CCharaBase::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackMouthCol->SetEnable(false);
	mpAttackHandCol->SetEnable(false);
	mpAttackScreamCol->SetEnable(false);
}