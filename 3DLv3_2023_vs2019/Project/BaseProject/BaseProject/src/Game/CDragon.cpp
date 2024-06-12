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

// コンストラクタ
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
	, mMotionBlurRemainTime(0.0f)
	, mAngle(0.0f)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

	// テーブル内のアニメーションデータを読み込み
	int size = DragonData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const DragonData::AnimData& data = DragonData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機のアニメーションを再生
	ChangeAnimation(EDragonAnimType::eIdle1);
	SetAnimationSpeed(0.4f);

	// ランダム値を生成して取得した値によりステータスを設定
	int rand = Math::Rand(0.0f, 5.0f);
	mStatus = ENEMY_STATUS[rand];
	mMaxStatus = mStatus;

	// 怒り値の最大値を設定
	// 最大値は最大HPの2割を基準として設定
	mAngryStandardValue = mMaxStatus.hp * 0.2;

	// 状態の設定
	mState = EState::eIdle;

	// コライダーの生成
	CreateCollider();

	// 火炎放射の発射位置の設定
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

// デストラクタ
CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);
	SAFE_DELETE(mpColliderLine3);
	// ダメージ用コライダー
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
	// 押し戻しようコライダー
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

// インスタンスを取得
CDragon* CDragon::Instance()
{
	return spInstance;
}

// ゲーム開始時にコライダーを生成
void CDragon::CreateCollider()
{
	/* 線分コライダーの設定 */
	// 接地判定用
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	// 壁との当たり判定用
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

	////* ダメージ判定用コライダーの設定 *////
	/* 頭部 */
	// 頭
	mpHeadDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, -1.0f, 0.0f), CVector(-0.3f, 0.6f, 0.0f), 9.0f);
	mpHeadDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpHeadDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpHeadDamageCol->SetMeat(140);
	const CMatrix* headcol = GetFrameMtx("Armature_Tongue02");
	mpHeadDamageCol->SetAttachMtx(headcol);
	// 首
	mpNeckDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.3f, -0.9f, 0.0f), CVector(0.1f, 1.1f, 0.0f), 9.0f);
	mpNeckDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpNeckDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpNeckDamageCol->SetMeat(110);
	const CMatrix* neckcol = GetFrameMtx("Armature_Neck02");
	mpNeckDamageCol->SetAttachMtx(neckcol);

	/* 胴体 */
	// 胴
	mpBodyDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, -1.0f, 0.0f), CVector(0.0f, 2.6f, 0.0f), 13.0f);
	mpBodyDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpBodyDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpBodyDamageCol->SetMeat(105);
	const CMatrix* bodycol = GetFrameMtx("Armature_Spine01");
	mpBodyDamageCol->SetAttachMtx(bodycol);
	// 尻尾(根本)
	mpTailDamageCol_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 1.8f, 0.0f), CVector(0.1f, -1.3f, 0.0f), 5.4f);
	mpTailDamageCol_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpTailDamageCol_Root->SetCollisionTags({ ETag::eWeapon });
	mpTailDamageCol_Root->SetMeat(105);
	const CMatrix* tailcol_root = GetFrameMtx("Armature_joint20");
	mpTailDamageCol_Root->SetAttachMtx(tailcol_root);
	// 尻尾(先端)
	mpTailDamageCol_Tip = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.2f, 3.0f, 0.0f), CVector(0.15f, -1.2f, 0.0f), 3.5f);
	mpTailDamageCol_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpTailDamageCol_Tip->SetCollisionTags({ ETag::eWeapon });
	mpTailDamageCol_Tip->SetMeat(100);
	const CMatrix* tailcol_tip = GetFrameMtx("Armature_joint22");
	mpTailDamageCol_Tip->SetAttachMtx(tailcol_tip);

	/* 足(根本) */
	// 右前足
	mpLegDamageCol_RF_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, -0.1f), 6.0f);
	mpLegDamageCol_RF_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF_Root->SetMeat(105);
	const CMatrix* legcol_rf_root = GetFrameMtx("Armature_UpperArm_R");
	mpLegDamageCol_RF_Root->SetAttachMtx(legcol_rf_root);
	// 左前足
	mpLegDamageCol_LF_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.1f), 6.0f);
	mpLegDamageCol_LF_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF_Root->SetMeat(105);
	const CMatrix* legcol_lf_root = GetFrameMtx("Armature_UpperArm_L");
	mpLegDamageCol_LF_Root->SetAttachMtx(legcol_lf_root);
	// 右後ろ足
	mpLegDamageCol_RB_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegDamageCol_RB_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB_Root->SetMeat(95);
	const CMatrix* legcol_rb_root = GetFrameMtx("Armature_UpperReg_R");
	mpLegDamageCol_RB_Root->SetAttachMtx(legcol_rb_root);
	// 左後ろ足
	mpLegDamageCol_LB_Root = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegDamageCol_LB_Root->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB_Root->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB_Root->SetMeat(95);
	const CMatrix* legcol_lb_root = GetFrameMtx("Armature_UpperLeg_L");
	mpLegDamageCol_LB_Root->SetAttachMtx(legcol_lb_root);

	/* 足 */
	// 右前足
	mpLegDamageCol_RF = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_RF->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF->SetMeat(105);
	const CMatrix* legcol_rf = GetFrameMtx("Armature_RowerArm_R");
	mpLegDamageCol_RF->SetAttachMtx(legcol_rf);
	// 左前足
	mpLegDamageCol_LF = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_LF->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF->SetMeat(105);
	const CMatrix* legcol_lf = GetFrameMtx("Armature_LowerArm_L");
	mpLegDamageCol_LF->SetAttachMtx(legcol_lf);
	// 右後ろ足
	mpLegDamageCol_RB = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_RB->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB->SetMeat(95);
	const CMatrix* legcol_rb = GetFrameMtx("Armature_RowerReg_R");
	mpLegDamageCol_RB->SetAttachMtx(legcol_rb);
	// 左後ろ足
	mpLegDamageCol_LB = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegDamageCol_LB->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB->SetMeat(95);
	const CMatrix* legcol_lb = GetFrameMtx("Armature_LowerLeg_L");
	mpLegDamageCol_LB->SetAttachMtx(legcol_lb);

	/* 足(足先) */
	// 右前足
	mpLegDamageCol_RF_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_RF_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RF_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RF_Tip->SetMeat(105);
	const CMatrix* legcol_rf_tip = GetFrameMtx("Armature_Middle01_R");
	mpLegDamageCol_RF_Tip->SetAttachMtx(legcol_rf_tip);
	// 左前足
	mpLegDamageCol_LF_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_LF_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LF_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LF_Tip->SetMeat(105);
	const CMatrix* legcol_lf_tip = GetFrameMtx("Armature_Middle01_L");
	mpLegDamageCol_LF_Tip->SetAttachMtx(legcol_lf_tip);
	// 右後ろ足
	mpLegDamageCol_RB_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_RB_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_RB_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_RB_Tip->SetMeat(95);
	const CMatrix* legcol_rb_tip = GetFrameMtx("Armature_MiddleToe01_R");
	mpLegDamageCol_RB_Tip->SetAttachMtx(legcol_rb_tip);
	// 左後ろ足
	mpLegDamageCol_LB_Tip = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpLegDamageCol_LB_Tip->SetCollisionLayers({ ELayer::eAttackCol });
	mpLegDamageCol_LB_Tip->SetCollisionTags({ ETag::eWeapon });
	mpLegDamageCol_LB_Tip->SetMeat(95);
	const CMatrix* legcol_lb_tip = GetFrameMtx("Armature_MiddleToe01_L");
	mpLegDamageCol_LB_Tip->SetAttachMtx(legcol_lb_tip);

	/* 翼 */


	////* 押し戻し用コライダーの設定 *////
	/* 頭部 */
	// 頭
	mpHeadCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, -1.0f, 0.0f), CVector(-0.3f, 0.6f, 0.0f), 9.0f);
	mpHeadCol->SetCollisionLayers({ ELayer::ePlayer });
	mpHeadCol->SetCollisionTags({ ETag::ePlayer });
	mpHeadCol->SetMeat(140);
	mpHeadCol->SetAttachMtx(headcol);
	// 首
	mpNeckCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.3f, -0.9f, 0.0f), CVector(0.1f, 1.1f, 0.0f), 9.0f);
	mpNeckCol->SetCollisionLayers({ ELayer::ePlayer });
	mpNeckCol->SetCollisionTags({ ETag::ePlayer });
	mpNeckCol->SetMeat(110);
	mpNeckCol->SetAttachMtx(neckcol);

	/* 胴体 */
	// 胴
	mpBodyCol = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, -1.0f, 0.0f), CVector(0.0f, 2.6f, 0.0f), 13.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });
	mpBodyCol->SetMeat(105);
	mpBodyCol->SetAttachMtx(bodycol);
	// 尻尾(根本)
	mpTailCol_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 1.8f, 0.0f), CVector(0.1f, -1.3f, 0.0f), 5.4f);
	mpTailCol_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpTailCol_Root->SetCollisionTags({ ETag::ePlayer });
	mpTailCol_Root->SetMeat(105);
	mpTailCol_Root->SetAttachMtx(tailcol_root);
	// 尻尾(先端)
	mpTailCol_Tip = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.2f, 3.0f, 0.0f), CVector(0.15f, -1.2f, 0.0f), 3.5f);
	mpTailCol_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpTailCol_Tip->SetCollisionTags({ ETag::ePlayer });
	mpTailCol_Tip->SetMeat(100);
	mpTailCol_Tip->SetAttachMtx(tailcol_tip);

	/* 足(根本) */
	// 右前足
	mpLegCol_RF_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, -0.1f), 6.0f);
	mpLegCol_RF_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF_Root->SetMeat(105);
	mpLegCol_RF_Root->SetAttachMtx(legcol_rf_root);
	// 左前足
	mpLegCol_LF_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.1f), 6.0f);
	mpLegCol_LF_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF_Root->SetMeat(105);
	mpLegCol_LF_Root->SetAttachMtx(legcol_lf_root);
	// 右後ろ足
	mpLegCol_RB_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegCol_RB_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB_Root->SetMeat(95);
	mpLegCol_RB_Root->SetAttachMtx(legcol_rb_root);
	// 左後ろ足
	mpLegCol_LB_Root = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.1f, 0.0f, 0.0f), CVector(-0.2f, 1.2f, 0.0f), 8.0f);
	mpLegCol_LB_Root->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB_Root->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB_Root->SetMeat(95);
	mpLegCol_LB_Root->SetAttachMtx(legcol_lb_root);

	/* 足 */
	// 右前足
	mpLegCol_RF = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_RF->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF->SetMeat(105);
	mpLegCol_RF->SetAttachMtx(legcol_rf);
	// 左前足
	mpLegCol_LF = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_LF->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF->SetMeat(105);
	mpLegCol_LF->SetAttachMtx(legcol_lf);
	// 右後ろ足
	mpLegCol_RB = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_RB->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB->SetMeat(95);
	mpLegCol_RB->SetAttachMtx(legcol_rb);
	// 左後ろ足
	mpLegCol_LB = new CColliderCapsule(this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.2f, 0.0f), 4.8f);
	mpLegCol_LB->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB->SetMeat(95);
	mpLegCol_LB->SetAttachMtx(legcol_lb);

	/* 足(足先) */
	// 右前足
	mpLegCol_RF_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_RF_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RF_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RF_Tip->SetMeat(105);
	mpLegCol_RF_Tip->SetAttachMtx(legcol_rf_tip);
	// 左前足
	mpLegCol_LF_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_LF_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LF_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LF_Tip->SetMeat(105);
	mpLegCol_LF_Tip->SetAttachMtx(legcol_lf_tip);
	// 右後ろ足
	mpLegCol_RB_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_RB_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_RB_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_RB_Tip->SetMeat(95);
	mpLegCol_RB_Tip->SetAttachMtx(legcol_rb_tip);
	// 左後ろ足
	mpLegCol_LB_Tip = new CColliderSphere(this, ELayer::eEnemy, 0.7f);
	mpLegCol_LB_Tip->SetCollisionLayers({ ELayer::ePlayer });
	mpLegCol_LB_Tip->SetCollisionTags({ ETag::ePlayer });
	mpLegCol_LB_Tip->SetMeat(95);
	mpLegCol_LB_Tip->SetAttachMtx(legcol_lb_tip);

	////* 攻撃用コライダーの設定 *////
	mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	const CMatrix* attackcol = GetFrameMtx("Armature_Tongue02");
	mpAttackMouthCol->SetAttachMtx(attackcol);

	// 最初は攻撃判定用のコライダーはオフにしておく
	mpAttackMouthCol->SetEnable(false);

}

// コライダーの更新処理
void CDragon::ColliderUpdate()
{
	////* ダメージ用コライダー *////
	/* 頭部 */
	mpHeadDamageCol->Update();	// 頭
	mpNeckDamageCol->Update();	// 首
	/* 胴体 */
	mpBodyDamageCol->Update();		// 胴体
	mpTailDamageCol_Root->Update();// 尻尾(根本)
	mpTailDamageCol_Tip->Update(); // 尻尾(先端)
	/* 足 */
	// 右前足
	mpLegDamageCol_RF_Root->Update();
	mpLegDamageCol_RF->Update();
	mpLegDamageCol_RF_Tip->Update();
	// 左前足
	mpLegDamageCol_LF_Root->Update();
	mpLegDamageCol_LF->Update();
	mpLegDamageCol_LF_Tip->Update();
	// 右後ろ足
	mpLegDamageCol_RB_Root->Update();
	mpLegDamageCol_RB->Update();
	mpLegDamageCol_RB_Tip->Update();
	// 左後ろ足
	mpLegDamageCol_LB_Root->Update();
	mpLegDamageCol_LB->Update();
	mpLegDamageCol_LB_Tip->Update();
	/* 翼 */

	////* 押し戻し用コライダー *////
	 /* 頭部 */
	mpHeadCol->Update();	// 頭
	mpNeckCol->Update();	// 首
	/* 胴体 */
	mpBodyCol->Update();		// 胴体
	mpTailCol_Root->Update();// 尻尾(根本)
	mpTailCol_Tip->Update(); // 尻尾(先端)
	/* 足 */
	// 右前足
	mpLegCol_RF_Root->Update();
	mpLegCol_RF->Update();
	mpLegCol_RF_Tip->Update();
	// 左前足
	mpLegCol_LF_Root->Update();
	mpLegCol_LF->Update();
	mpLegCol_LF_Tip->Update();
	// 右後ろ足
	mpLegCol_RB_Root->Update();
	mpLegCol_RB->Update();
	mpLegCol_RB_Tip->Update();
	// 左後ろ足
	mpLegCol_LB_Root->Update();
	mpLegCol_LB->Update();
	mpLegCol_LB_Tip->Update();
}

// アニメーションの切り替え
void CDragon::ChangeAnimation(EDragonAnimType type)
{
	if (!(EDragonAnimType::None < type && type < EDragonAnimType::Num)) return;
	DragonData::AnimData data = DragonData::GetAnimData((int)type);
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
	SetAnimationSpeed(1.0f);
}

// 怒り状態の切り替え処理
void CDragon::ChangeAngry()
{
	// 怒り値を基準値に合わせる
	mAngryValue = mAngryStandardValue;
	mIsAngry = true;
	// 怒り状態への移行時に咆哮攻撃を行う
	ChangeAnimation(EDragonAnimType::eScream);
	SetAnimationSpeed(0.5f);
	// 各ステータスを強化
	mStatus.atk += 10;
	mStatus.def += 10;
	// 怒り状態への移行フラグを元に戻す
	mChangeAngry = false;

	// UpdateBattle_Idle()内で使用ていた
	// 変数を初期化しておく
	mElapsedTime = 0.0f;
	mRandSave = 0;
	// 戦闘の段階は攻撃処理にする
	mBatteleStep = 2;

	// モーションブラーを掛けている最中であれば、
	// 新しくモーションブラーを掛け直さない
	if (mMotionBlurRemainTime <= 0.0f)
	{
		System::SetEnableMotionBlur(true);
		mMotionBlurRemainTime = MOTION_BLUR_TIME;
	}
}

// モーションブラーの更新処理
void CDragon::UpdateMotionBlur()
{
	// モーションブラーの残り時間が残っていなければ、処理しない
	if (mMotionBlurRemainTime <= 0.0f) return;
	// 現在のカメラを取得し、存在しなければ処理しない
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// カメラの向きと反対方向へブラーを掛けるため、
	// 反転したカメラの向きを取得
	CVector camDir = -currentCamera->VectorZ();

	// 残り時間から経過時間の割合を取得（経過時間の割合 = 1 - 残り時間の割合）
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// ブラーの幅をサインカーブで経過時間に合わせて増減させる
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// モーションブラーのパラメータを設定
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// 残り時間を経過時間分減少させる
	mMotionBlurRemainTime -= Time::DeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

// プレイヤーを見つけたかどうか
bool CDragon::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 視野角度の判定
	// 自身からプレイヤーまでのベクトルを取得
	CVector EP = (playerPos - enemyPos).Normalized();
	// 自身の正面方向のベクトルを取得
	CVector forward = VectorZ().Normalized();
	// 正面方向のベクトルとプレイヤーまでの
	// 内積から角度を求める
	float dotZ = forward.Dot(EP);

	// 求めた角度が視野角度外の場合、falseを返す
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	// 距離の判定
	// 自身からプレイヤーまでの距離を求める
	float distance = (playerPos - enemyPos).Length();
	// 求めた距離が視野距離よりも遠い場合、falseを返す
	if (distance > FOV_LANGE) return false;

	// 視野判定と距離判定を通ったのでtrueを返す
	return true;
}

// バックステップできるかどうか
bool CDragon::IsBackStep() const
{
	// レイの開始地点
	CVector startPos = Position() + CVector(0.0f,50.0f,0.0f);
	// レイの終了地点
	CVector endPos = Position() + -VectorZ().Normalized() * BACKSTEP_RAY + CVector(0.0f, 50.0f, 0.0f);
	// 衝突位置までの距離返却用
	float outDist = 0.0f;

	// フィールドとレイの当たり判定行う
	if (gField->CollisionRay(startPos, endPos, &outDist))
	{
		// 衝突位置までの長さを表示(デバッグ)
		CDebugPrint::Print("%.0f\n", outDist);
		return false;
	};

	// レイの長さを表示
	CDebugPrint::Print("%.0f\n", outDist);
	// 後ろに十分なスペースがあるので
	// バックステップが可能
	return true;
}


//プレイヤーとの距離を取得
CDragon::EDistanceType CDragon::PlayerFromDistance()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 距離の判定
	float distance = (playerPos - enemyPos).Length();
	// 距離と判別用の値を比べて、距離を判別
	if (distance >= 280.0f) mDistanceType = EDistanceType::eFar;// 遠距離
	else if (distance >= 230.0f) mDistanceType = EDistanceType::eMedium;// 中距離
	else mDistanceType = EDistanceType::eNear;// 近距離

	// 判別した距離を返す
	return mDistanceType;
}

// 更新処理
void CDragon::Update()
{
	// 状態に合わせて更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 戦闘状態
	case EState::eBattle:
		UpdateBattle();
		break;
		// 必殺技攻撃
	case EState::eSpAttack:
		UpdateSpecalAttack();
		break;
		// 死亡状態
	case EState::eDeath:
		break;
	}

	// 重力の設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// 攻撃中または、バックステップ以外の時は移動方向へ向ける
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
			// 空中移動の時のみ補完割合を調整
			if (AnimationIndex() == (int)EDragonAnimType::eFlyForward)
			{
				t = 0.2f;
			}

			CVector forward = CVector::Slerp(current, target, t);
			Rotation(CQuaternion::LookRotation(forward));
		}
	}
	CXCharacter::Update();
	// 各コライダーの更新処理
	ColliderUpdate();
	// モーションブラーの更新処理
	UpdateMotionBlur();
#ifdef _DEBUG
	// ドラゴンのステータスを表示
	CDebugPrint::Print("敵ステータス\n");
	CDebugPrint::Print("レベル : %d\nHP : %d\n攻撃力 : %d\n防御力 : %d\n",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);
	// 怒り状態かどうか
	mIsAngry ? CDebugPrint::Print("怒り状態\n") : CDebugPrint::Print("非怒り状態\n");
	// ダメージを表示
	CDebugPrint::Print("%d\n", mDamage);
	//// プレイヤーとの距離を表示
	//CVector pPos = CPlayer::Instance()->Position();
	//CVector ePos = Position();
	//float distance = (pPos - ePos).Length();
	//CDebugPrint::Print("プレイヤーとの距離 : %.1f\n", distance);

	//CDebugPrint::Print("プレイヤーとの距離\n");
	//if (mDistanceType == EDistanceType::eNear) CDebugPrint::Print("近距離\n");
	//else if (mDistanceType == EDistanceType::eMedium) CDebugPrint::Print("中距離\n");
	//else CDebugPrint::Print("遠距離\n");

	//if (CInput::PushKey('1'))
	//{
	//	int p = GetHPPercent();
	//	printf("%d\n", p);
	//}

	// バックステップ判定用
	//if (IsBackStep()) CDebugPrint::Print("true\n");
	//else CDebugPrint::Print("false\n");

#endif
		//「B」キーを押したら、モーションブラー開始
	if (CInput::PushKey('B'))
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 地面との当たり判定
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
			mIsGrounded = true;
		}
	}
	// 壁との当たり判定
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
		}
	}

	// 衝突したコライダーが攻撃判定用のコライダーであれば
	//if (self == mpAttackMouthCol)
	//{
	//	CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
	//	// 相手のコライダーの持ち主がキャラであれば
	//	if (chara != nullptr)
	//	{
	//		// ダメージを与える
	//		CPlayer* player = CPlayer::Instance();
	//		int atk = Status().atk;
	//		int def = player->Status().def;
	//		float motionvalue = GetMotionValue();
	//		// ダメージ計算
	//		int damage = player->TakePlayerToDamage(atk, def, motionvalue);

	//		// 既に攻撃済みのキャラでなければ
	//		if (!IsAttackHitObj(chara))
	//		{
	//			// ダメージを与える
	//			chara->TakeDamage(damage);

	//			// 攻撃済みのリストに追加
	//			AddAttackHitObj(chara);
	//		}
	//	}
	//}
}

// 描画処理
void CDragon::Render()
{
	CXCharacter::Render();
#ifdef _DEBUG

	// 視野判定用
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

// ダメージ処理
void CDragon::TakeDamage(int damage)
{
	mDamage = damage;
	// ダメージ分HPを減少
	mStatus.hp -= damage;
	// ダメージの一割分を怯み値に加算
	mFearValue += damage / 10;

	// 怒り状態で無ければ、ダメージ分怒り値増加
	if (!mIsAngry) mAngryValue += damage;
	// 怒り状態で有れば、ダメージの半分の値だけ怒り値を減少
	else mAngryValue -= damage / 2;

	// HPが0になったら死亡処理を行う
	if (mStatus.hp <= 0)
	{
		// 死亡処理
		mState = EState::eDeath;
		ChangeAnimation(EDragonAnimType::eDie);
	}
}

// 攻撃開始
void CDragon::AttackStart()
{
	CCharaBase::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eAttackMouth:// 噛みつき攻撃
		//mpAttackMouthCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eAttackHand:// 飛び掛かり攻撃
		break;
	}
}

// 攻撃終了
void CDragon::AttackEnd()
{
	CCharaBase::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	//mpAttackMouthCol->SetEnable(false);
}