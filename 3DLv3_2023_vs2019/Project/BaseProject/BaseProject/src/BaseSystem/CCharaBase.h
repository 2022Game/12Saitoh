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

	// �L�����̌��݃X�e�[�^�X���擾
	const CharaStatus& Status() const;

	virtual void TakeDamage(int damage);

private:
	int mDamage; // �_���[�W�l

protected:
	CharaStatus mStatus;	// �L�����N�^�[�X�e�[�^�X
	int mTemporaryDamage;	// �b��_���[�W
};
