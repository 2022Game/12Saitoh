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

	/* �_���[�W�v�Z */
	// �v���C���[���G�ɗ^����_���[�W�v�Z
	int TakeEnemyToDamage();
	// �G���v���C���[�ɗ^����_���[�W�v�Z
	int TakePlayerToDamage(const int atk, const int def, const float motionvalue);

	// �_���[�W�y�����̌v�Z
	float DamageReductionRate(int def);

	// �_���[�W�l�̎擾
	int GetDamage() const;
	// �b��_���[�W�l�̎擾
	int GetTemporaryDamage() const;

	// �L�����̌��݃X�e�[�^�X���擾
	const CharaStatus& Status() const;

private:
	int mDamage; // �_���[�W�l
protected:
	CharaStatus mStatus;	// �L�����N�^�[�X�e�[�^�X
	int mTemporaryDamage;	// �b��_���[�W
};
