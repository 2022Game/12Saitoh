#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	/// <summary>
	/// �v���C���[���G�ɗ^����_���[�W
	/// </summary>
	/// <param name="atk">�v���C���[�̍U����</param>
	/// <param name="def">�G�̖h���</param>
	/// <param name="motionvalue">���[�V�����l</param>
	/// <param name="multiplier">�����{��</param>
	/// <returns>�_���[�W</returns>
	int TakeEnemyToDamage(const int atk, const int def, const float motionvalue, const float multiplier);
	/// <summary>
	/// �G���v���C���[�ɗ^����_���[�W
	/// </summary>
	/// <param name="atk">�G�̍U����</param>
	/// <param name="def">�v���C���[�̖h���</param>
	/// <param name="motionvalue">���[�V�����l</param>
	/// <returns>��_���[�W�l</returns>
	int TakePlayerToDamage(const int atk, const int def, const float motionvalue);

	// �_���[�W�y�����̌v�Z
	float DamageReductionRate(int def);

	// �_���[�W�l�̎擾
	int GetDamage() const;
	// �b��_���[�W�l�̎擾
	int GetTemporaryDamage() const;

	// �_���[�W����
	virtual void TakeDamage(int damage);

	/// <summary>
	/// �U���J�n
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// �U���I��
	/// </summary>
	virtual void AttackEnd();

private:
	int mDamage; // �_���[�W�l

protected:
	/// <summary>
	/// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	std::list<CObjectBase*> mAttackHitObjects;

	CharaStatus mMaxStatus;		// �L�����̍ő�X�e�[�^�X
	CharaStatus mStatus;		// �L�����̌��݂̃X�e�[�^�X

	int mTemporaryDamage;	// �b��_���[�W
};
