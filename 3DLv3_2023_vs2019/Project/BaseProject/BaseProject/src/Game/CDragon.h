#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"

class CFlamethrower;
class CSPFlamethrower;
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

private:
	// �ҋ@���
	void UpdateIdle();
	// �퓬���
	void UpdateBattle();
	// �퓬���̃A�C�h������
	void UpdateBattele_Idle();
	// �퓬���̒ǐՏ���
	void UpdateBattle_Chase();
	// �U������
	void UpdateAttack();
	// �󒆃u���X�U������
	void UpdateSpecalAttack();

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
	
	// �v���C���[�����������ǂ���
	bool IsFoundPlayer() const;
	// �o�b�N�X�e�b�v�ł��邩�ǂ���
	bool IsBackStep() const;

	// �c��HP�̊������擾
	int GetHPPercent() const;
	// �퓬�A�C�h�����̃����_���l�����p
	// 1�`3�܂ł̗�����Ԃ�
	int BattleIdleRand() const;

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
	CColliderSphere* mpBodyCol;		//�����߂��p�R���C�_�[
	CColliderSphere* mpDamageCol;	// �_���[�W�p�R���C�_�[
	CColliderSphere* mpAttackMouthCol;// ���݂��U���p�R���C�_�[

	CFlamethrower* mpFlamethrower; // �Ή�����(�n��)
	CSPFlamethrower* mpSpFlamethrower; // �Ή�����(��)
	CVector mMoveSpeed;			// �ړ����x
	CVector mSaveDestination;	// �ړI�n�̕ۑ��p
	CVector mSaveVec;			// �x�N�g���̕ۑ��p

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsAngry;		// �{���Ԃ��ǂ���
	bool mChangeAngry;	// �{���ԂɈڍs�����ǂ���

	int mAngryStandardValue;// �{��l�̊�l
	int mAngryValue;	// �{��l
	int mFearValue;		// ���ݒl
	int mRandSave;		// �����ۑ��p
	int mBatteleStep;	// �퓬�̒i�K
	int mAttackStep;	// �U���̒i�K
	int mSpAttackStep;	// �K�E�Z�U���̍U���i�K
	int mSpAttackNum;	// �K�E�Z�U�����s�����񐔂��L��

	float mElapsedTime;		// �o�ߎ��Ԍv����
	float mAngryElapsedTime;// �{��o�ߎ��Ԍv����
	float mChaseElapsedTime;// �ړ��o�ߎ��Ԍv����
	float mAngle;	// �X�e�[�W���S���猩���h���S���̊p�x
};
#endif