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

CDragon* CDragon::spInstance = nullptr;

// �R���X�g���N�^
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mSaveVec(CVector::zero)
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
	

	/* �U���p�R���C�_�[�̐ݒ� */
	//mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	//mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	//mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	//// �R���C�_�[���A�^�b�`
	//const CMatrix* attackcol = GetFrameMtx("Armature_Tongue02");
	//mpAttackMouthCol->SetAttachMtx(attackcol);

	//// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	//mpAttackMouthCol->SetEnable(false);

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

	////* �����߂��p�R���C�_�[ *////
	 /* ���� */
	mpHeadCol->Update();	// ��
	mpNeckCol->Update();	// ��
	/* ���� */
	mpBodyCol->Update();		// ����
	mpTailCol_Root->Update();// �K��(���{)
	mpTailCol_Tip->Update(); // �K��(��[)
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
	CVector startPos = Position() + CVector(0.0f,50.0f,0.0f);
	// ���C�̏I���n�_
	CVector endPos = Position() + -VectorZ().Normalized() * BACKSTEP_RAY + CVector(0.0f, 50.0f, 0.0f);
	// �Փˈʒu�܂ł̋����ԋp�p
	float outDist = 0.0f;

	// �t�B�[���h�ƃ��C�̓����蔻��s��
	if (gField->CollisionRay(startPos, endPos, &outDist))
	{
		// �Փˈʒu�܂ł̒�����\��(�f�o�b�O)
		CDebugPrint::Print("%.0f\n", outDist);
		return false;
	};

	// ���C�̒�����\��
	CDebugPrint::Print("%.0f\n", outDist);
	// ���ɏ\���ȃX�y�[�X������̂�
	// �o�b�N�X�e�b�v���\
	return true;
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
	ColliderUpdate();
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

#endif
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
	//if (self == mpAttackMouthCol)
	//{
	//	CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
	//	// ����̃R���C�_�[�̎����傪�L�����ł����
	//	if (chara != nullptr)
	//	{
	//		// �_���[�W��^����
	//		CPlayer* player = CPlayer::Instance();
	//		int atk = Status().atk;
	//		int def = player->Status().def;
	//		float motionvalue = GetMotionValue();
	//		// �_���[�W�v�Z
	//		int damage = player->TakePlayerToDamage(atk, def, motionvalue);

	//		// ���ɍU���ς݂̃L�����łȂ����
	//		if (!IsAttackHitObj(chara))
	//		{
	//			// �_���[�W��^����
	//			chara->TakeDamage(damage);

	//			// �U���ς݂̃��X�g�ɒǉ�
	//			AddAttackHitObj(chara);
	//		}
	//	}
	//}
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
	}
}

// �U���J�n
void CDragon::AttackStart()
{
	CCharaBase::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eAttackMouth:// ���݂��U��
		//mpAttackMouthCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eAttackHand:// ��ъ|����U��
		break;
	}
}

// �U���I��
void CDragon::AttackEnd()
{
	CCharaBase::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	//mpAttackMouthCol->SetEnable(false);
}