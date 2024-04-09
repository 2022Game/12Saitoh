#ifndef CDRAGON_H
#define CDRAGON_H
#include "DragonData.h"
#include "CXCharacter.h"
#include "CCollider.h"

class CFlamethrower;
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

	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EDragonAnimType type);

	// �v���C���[�����������ǂ���
	bool IsFoundPlayer() const;

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

		eIdle,	// �ҋ@
		eBattle,// �퓬
		eDeath,	// ���S
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

	CFlamethrower* mpFlamethrower; // �Ή�����
	CVector mMoveSpeed;	// �ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	bool mIsAngry;		// �{���Ԃ��ǂ���

	int mAngryStandardValue;// �{��l�̊�l
	int mAngryValue;	// �{��l
	int mRandSave;		// �����ۑ��p
	int mBatteleStep;	// �퓬�̒i�K

	float mElapsedTime;		// �o�ߎ��Ԍv����
	float mAngryElapsedTime;// �{��o�ߎ��Ԍv����
	float mChaseElapsedTime;// �ړ��o�ߎ��Ԍv����
};
#endif