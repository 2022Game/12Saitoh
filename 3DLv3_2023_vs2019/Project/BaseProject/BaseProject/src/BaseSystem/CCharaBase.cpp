#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mDamage(0)
	, mTemporaryDamage(0)
{
}

CCharaBase::~CCharaBase()
{
}

// �v���C���[���G�ɗ^����_���[�W�v�Z
int CCharaBase::TakeEnemyToDamage()
{
	return 0;
}

// �G���v���C���[�ɗ^����_���[�W�v�Z
int CCharaBase::TakePlayerToDamage(const int atk, const int def, const float motionvalue)
{
	// �_���[�W�v�Z
	// ��_���[�W = �U���� �~ ���[�V�����l �~ �_���[�W�y����
	mDamage = atk * motionvalue * DamageReductionRate(def);
	// ��_���[�W����b��_���[�W���v�Z
	mTemporaryDamage = mDamage * 0.5f;

	return mDamage;
}

// �_���[�W�y�����̌v�Z
float CCharaBase::DamageReductionRate(int def)
{
	float percent = 80.0f / (80.0f + (float)def) * 100.0f;
	// �p�[�Z���e�[�W�������ɒ���
	percent /= 100.0f;
	return percent;
}

// �_���[�W�l�̎擾
int CCharaBase::GetDamage() const
{
	return mDamage;
}

// �b��_���[�W�l�̎擾
int CCharaBase::GetTemporaryDamage() const
{
	return mTemporaryDamage;
}

// �L�����̌��݃X�e�[�^�X���擾
const CharaStatus& CCharaBase::Status() const
{
	return mStatus;
}