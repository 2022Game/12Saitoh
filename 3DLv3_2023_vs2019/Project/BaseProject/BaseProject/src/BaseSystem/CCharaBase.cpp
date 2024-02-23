#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mDamage(0)
	, mTemporaryDamage(0)
	, mStatus(DEFAULT[0])
{
}

CCharaBase::~CCharaBase()
{
}

// �v���C���[���G�ɗ^����_���[�W�v�Z
int CCharaBase::TakeEnemyToDamage(const int atk, const int def, const float motionvalue, float const multiplier)
{
	// �_���[�W�v�Z
	// �_���[�W  = ((�U���� �~ ���[�V�����l) - (�h���/2)) �~ �����{��
	float damage = ((atk * motionvalue) - (def / 2)) * multiplier;
	mDamage = static_cast<int>(damage);
	// �����ɕϊ�
	if (mDamage < 0) mDamage = -mDamage;

	return mDamage;
}

// �G���v���C���[�ɗ^����_���[�W�v�Z
int CCharaBase::TakePlayerToDamage(const int atk, const int def, const float motionvalue)
{
	// �_���[�W�v�Z
	// ��_���[�W = �U���� �~ ���[�V�����l �~ �_���[�W�y����
	float damage = atk * motionvalue * DamageReductionRate(def);
	mDamage = static_cast<int>(damage);

	// ��_���[�W����b��_���[�W���v�Z
	float temporarydamage = mDamage * 0.5f;
	mTemporaryDamage = static_cast<int>(temporarydamage);
	// �����ɕϊ�
	if (mDamage < 0) mDamage = -mDamage;

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

void CCharaBase::TakeDamage(int damage)
{
	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		// ���S����
	}
}