#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"
#include "CSound.h"
#include "CCutin_GameClear.h"

class CFlamethrower;
class CSPFlamethrower;
class CHPGauge_Dragon;
/*
�h���S���N���X
�L�����N�^�N���X���p��
*/
class CDragon : public CXCharacter
{
public:
	// �C���X�^���X���擾
	static CDragon* Instance();
	// �R���X�g���N�^
	CDragon();
	// �f�X�g���N�^
	~CDragon();

	// �X�V����
	void Update();
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`�揈��
	void Render();

	// �_���[�W����
	void TakeDamage(int damage)override;
	/// <summary>
	/// �U���J�n
	/// </summary>
	void AttackStart() override;
	/// <summary>
	/// �U���I��
	/// </summary>
	void AttackEnd() override;

	// �v���C���[�����������ǂ���
	bool IsFoundPlayer() const;
	// �󒆃u���X�����ǂ���
	bool IsFlyBreath() const;
	// ���S�������ǂ���
	bool IsDie() const;
	// �󒆃u���X�̍U����
	int SpAttackNum() const;

private:
	// �ҋ@���
	void UpdateIdle();
	// �퓬���
	void UpdateBattle();
	// ���S���
	void UpdateDie();
	// �퓬���̃A�C�h������
	void UpdateBattele_Idle();
	// �퓬���̒ǐՏ���
	void UpdateBattle_Chase();
	// �U������
	void UpdateAttack();
	// ���ݒ��̏���
	void UpdateBattele_Fear();
	// �󒆃u���X�U������
	void UpdateSpecalAttack();

	// ���[�V�����u���[����
	void UpdateMotionBlur();

	/* �ʏ�U���e���� */
	// ���K�U��
	void Update_Sceream();
	// ���݂��U��
	void Update_AttackMouth();
	// ��ъ|����U��
	void Update_AttackHand();
	// �n��u���X�U��
	void Update_AttackFlame();
	// �o�b�N�X�e�b�v
	void Update_BackStep();

	/* �󒆃u���X�U���e���� */
	// ���K����
	void UpdateSpAttack_Step0();
	// ��������
	void UpdateSpAttack_Step1();
	// �󒆈ړ�����1
	void UpdateSpAttack_Step2();
	// �󒆈ړ�����2
	void UpdateSpAttack_Step3();
	// �󒆃A�C�h���y�э�������
	void UpdateSpAttack_Step4();
	// �󒆃u���X�U��
	void UpdateSpAttack_Step5();
	// �󒆃A�C�h������
	void UpdateSpAttack_Step6();
	//  �󒆈ړ�����3
	void UpdateSpAttack_Step7();
	// ��������
	void UpdateSpAttack_Step8();
	// �ړI�n���擾
	CVector GetDestination() const;
	// �p�x�̎擾
	float GetAngle() const;


	// �{���Ԃ̐؂�ւ�����
	void ChangeAngry();
	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EDragonAnimType type);
	// �Q�[���J�n���ɃR���C�_�[�𐶐�
	void CreateCollider();
	// �R���C�_�[�̍X�V����
	void ColliderUpdate();
	// ���ׂăR���C�_�[�̖�����
	void DisableCollider();

	// �o�b�N�X�e�b�v�ł��邩�ǂ���
	bool IsBackStep() const;

	// �c��HP�̊������擾
	int GetHPPercent() const;
	// �퓬�A�C�h�����̃����_���l�����p
	// 1�`3�܂ł̗�����Ԃ�
	int BattleIdleRand() const;

	// ���C���΂��Ĉړ��ł���p�x���擾
	CVector GetRayAngleVec();

	// �v���C���[�Ƃ̋���
	enum class EDistanceType
	{
		None = -1,

		eNear,	// �ߋ���
		eMedium,// ������
		eFar,	// ������
	};
	EDistanceType mDistanceType; // �v���C���[�Ƃ̋���
	EDistanceType PlayerFromDistance(); // �v���C���[�Ƃ̋������擾

	// �G�̏��
	enum class EState
	{
		None = -1,

		eIdle,		// �ҋ@
		eBattle,	// �퓬
		eSpAttack,	// �K�E�Z�U��
		eDeath,		// ���S
	};
	EState mState;	// �G�̏��

	// �C���X�^���X
	static CDragon* spInstance;

	CColliderLine* mpColliderLine;	// �n�ʂƂ̓����蔻��p
	CColliderLine* mpColliderLine2;	// �ǂƂ̓����蔻��p
	CColliderLine* mpColliderLine3;	// �ǂƂ̓����蔻��p

	////* �_���[�W����p�R���C�_�[ *////
	/* ���� */
	CColliderCapsule* mpHeadDamageCol;	// ��
	CColliderCapsule* mpNeckDamageCol;	// ��
	/* ���� */
	CColliderCapsule* mpBodyDamageCol;		// ����
	CColliderCapsule* mpTailDamageCol_Root;	// �K��(���{)
	CColliderCapsule* mpTailDamageCol_Tip;	// �K��(��[)
	/* ��(���{) */
	CColliderCapsule* mpLegDamageCol_RF_Root; // �E�O��
	CColliderCapsule* mpLegDamageCol_LF_Root; // ���O��
	CColliderCapsule* mpLegDamageCol_RB_Root; // �E��둫
	CColliderCapsule* mpLegDamageCol_LB_Root; // ����둫
	/* �� */
	CColliderCapsule* mpLegDamageCol_RF;	// �E�O��
	CColliderCapsule* mpLegDamageCol_LF;	// ���O��
	CColliderCapsule* mpLegDamageCol_RB;	// �E��둫
	CColliderCapsule* mpLegDamageCol_LB;	// ����둫
	/* ��(����) */
	CColliderSphere* mpLegDamageCol_RF_Tip; // �E�O
	CColliderSphere* mpLegDamageCol_LF_Tip; // ���O
	CColliderSphere* mpLegDamageCol_RB_Tip; // �E��둫
	CColliderSphere* mpLegDamageCol_LB_Tip; // ����둫
	/* �� */
	// �E��
	CColliderLine* mpWingCol_R01;
	CColliderLine* mpWingCol_R02;
	CColliderLine* mpWingCol_R03;
	CColliderLine* mpWingCol_R04;
	CColliderLine* mpWingCol_R05;
	CColliderLine* mpWingCol_R06;
	// ����
	CColliderLine* mpWingCol_L01;
	CColliderLine* mpWingCol_L02;
	CColliderLine* mpWingCol_L03;
	CColliderLine* mpWingCol_L04;
	CColliderLine* mpWingCol_L05;
	CColliderLine* mpWingCol_L06;

	////* �����߂��p�R���C�_�[ *////
	/* ���� */
	CColliderCapsule* mpHeadCol;	// ��
	CColliderCapsule* mpNeckCol;	// ��
	/* ���� */
	CColliderCapsule* mpBodyCol;	// ����
	CColliderCapsule* mpTailCol_Root;// �K��(���{)
	CColliderCapsule* mpTailCol_Tip;// �K��(��[)
	/* ��(���{) */
	CColliderCapsule* mpLegCol_RF_Root; // �E�O��
	CColliderCapsule* mpLegCol_LF_Root;	// ���O��
	CColliderCapsule* mpLegCol_RB_Root;	// �E��둫
	CColliderCapsule* mpLegCol_LB_Root;	// ����둫
	/* �� */
	CColliderCapsule* mpLegCol_RF;	// �E�O��
	CColliderCapsule* mpLegCol_LF;	// ���O��
	CColliderCapsule* mpLegCol_RB;	// �E��둫
	CColliderCapsule* mpLegCol_LB;	// ����둫
	/* ��(����) */
	CColliderSphere* mpLegCol_RF_Tip; // �E�O
	CColliderSphere* mpLegCol_LF_Tip; // ���O
	CColliderSphere* mpLegCol_RB_Tip; // �E��둫
	CColliderSphere* mpLegCol_LB_Tip; // ����둫

	////* �U���p�̃R���C�_�[ *////
	CColliderSphere* mpAttackMouthCol;	// ���݂��U��
	CColliderSphere* mpAttackHandCol;	// ��ъ|����U��
	CColliderSphere* mpAttackScreamCol;	// ���K�U��

	CCutIn_GameClear* mpCutIn_GameClear;
	CFlamethrower* mpFlamethrower; // �Ή�����(�n��)
	CSPFlamethrower* mpSpFlamethrower; // �Ή�����(��)
	CVector mMoveSpeed;			// �ړ����x
	CVector mSaveDestination;	// �ړI�n�̕ۑ��p
	CVector mSaveVec;			// �x�N�g���̕ۑ��p
	CVector mRayAngleVec;		
	CSound* mpFlySE;
	CSound* mpJumpSE;
	CSound* mpJumpEndSE;
	CSound* mpBackSE;

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsAngry;		// �{���Ԃ��ǂ���
	bool mChangeAngry;	// �{���ԂɈڍs�����ǂ���
	bool mIsFlyBreath;	// �󒆃u���X�����ǂ���
	bool mIsDie;		// ���S�t���O

	int mAngryStandardValue;// �{��l�̊�l
	int mAngryValue;	// �{��l
	int mFearValue;		// ���ݒl
	int mRandSave;		// �����ۑ��p
	int mBatteleStep;	// �퓬�����̒i�K
	int mAttackStep;	// �U�������̒i�K
	int mFearStep;		// ���ݏ����̒i�K
	int mSpAttackStep;	// �K�E�Z�U���̍U���i�K
	int mSpAttackNum;	// �K�E�Z�U�����s�����񐔂��L��

	float mElapsedTime;		// �o�ߎ��Ԍv����
	float mAngryElapsedTime;// �{��o�ߎ��Ԍv����
	float mChaseElapsedTime;// �ړ��o�ߎ��Ԍv����
	float mFearElapsedTime; // ���ݒ��̌o�ߎ��Ԍv����
	float mAngle;			// �X�e�[�W���S���猩���h���S���̊p�x
	float mMotionBlurRemainTime; // ���[�V�����u���[���|����c�莞��

#ifdef _DEBUG
	class CRayAngleData
	{
	public:
		float dist;
		CVector start;
		CVector end;
	};
	std::vector<CRayAngleData> mRayAngleData;
#endif
	// �f�o�b�O�p
	int mDamage = 0;
};
#endif