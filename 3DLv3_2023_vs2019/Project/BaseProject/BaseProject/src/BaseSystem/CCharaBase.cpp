#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mStatus(DEFAULT[0])
	, mDamage(0)
	, mTemporaryDamage(0)
{
}

CCharaBase::~CCharaBase()
{
}

// �v���C���[���G�ɗ^����_���[�W�v�Z
int CCharaBase::TakeEnemyToDamage(const int atk, const int def, const float motionvalue, const int meat)
{
	// �����������l�ɕϊ�
	float meetvalue = meat / 100.0f;
	// �_���[�W�v�Z
	// �_���[�W  = ( (�U���� �~ ���[�V�����l) - (�h��� / 2) ) �~ �����{��
	float damage = ((atk * motionvalue) - (def / 2)) * meetvalue;
	mDamage = static_cast<int>(damage);
	// �_���[�W��0�ȉ��̏ꍇ�A�Œ�ۏ�_���[�W��Ԃ�
	if (mDamage < 0) return 10;

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

void CCharaBase::TakeDamage(int damage)
{
	mStatus.hp -= damage;
}

// �U���J�n
void CCharaBase::AttackStart()
{
	mAttackHitObjects.clear();
}

// �U���I��
void CCharaBase::AttackEnd()
{

}

// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
void CCharaBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
bool CCharaBase::IsAttackHitObj(CObjectBase* obj) const
{
	// ���Ƀ��X�g�ɒǉ����Ă��邩���m�F����
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}