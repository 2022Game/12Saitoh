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

// プレイヤーが敵に与えるダメージ計算
int CCharaBase::TakeEnemyToDamage(const int atk, const int def, const float motionvalue, const int meat)
{
	// 肉質を小数値に変換
	float meetvalue = meat / 100.0f;
	// ダメージ計算
	// ダメージ  = ( (攻撃力 × モーション値) - (防御力 / 2) ) × 肉質倍率
	float damage = ((atk * motionvalue) - (def / 2)) * meetvalue;
	mDamage = static_cast<int>(damage);
	// ダメージが0以下の場合、最低保障ダメージを返す
	if (mDamage < 0) return 10;

	return mDamage;
}

// 敵がプレイヤーに与えるダメージ計算
int CCharaBase::TakePlayerToDamage(const int atk, const int def, const float motionvalue)
{
	// ダメージ計算
	// 被ダメージ = 攻撃力 × モーション値 × ダメージ軽減率
	float damage = atk * motionvalue * DamageReductionRate(def);
	mDamage = static_cast<int>(damage);

	// 被ダメージから暫定ダメージを計算
	float temporarydamage = mDamage * 0.5f;
	mTemporaryDamage = static_cast<int>(temporarydamage);
	// 正数に変換
	if (mDamage < 0) mDamage = -mDamage;

	return mDamage;
}

// ダメージ軽減率の計算
float CCharaBase::DamageReductionRate(int def)
{
	float percent = 80.0f / (80.0f + (float)def) * 100.0f;
	// パーセンテージを少数に直す
	percent /= 100.0f;
	return percent;
}

// ダメージ値の取得
int CCharaBase::GetDamage() const
{
	return mDamage;
}

// 暫定ダメージ値の取得
int CCharaBase::GetTemporaryDamage() const
{
	return mTemporaryDamage;
}

void CCharaBase::TakeDamage(int damage)
{
	mStatus.hp -= damage;
}

// 攻撃開始
void CCharaBase::AttackStart()
{
	mAttackHitObjects.clear();
}

// 攻撃終了
void CCharaBase::AttackEnd()
{

}

// 攻撃がヒットしたオブジェクトを追加
void CCharaBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CCharaBase::IsAttackHitObj(CObjectBase* obj) const
{
	// 既にリストに追加しているかを確認する
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}