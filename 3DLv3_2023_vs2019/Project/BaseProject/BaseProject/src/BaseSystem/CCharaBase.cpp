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

// プレイヤーが敵に与えるダメージ計算
int CCharaBase::TakeEnemyToDamage()
{
	return 0;
}

// 敵がプレイヤーに与えるダメージ計算
int CCharaBase::TakePlayerToDamage(const int atk, const int def, const float motionvalue)
{
	// ダメージ計算
	// 被ダメージ = 攻撃力 × モーション値 × ダメージ軽減率
	mDamage = atk * motionvalue * DamageReductionRate(def);
	// 被ダメージから暫定ダメージを計算
	mTemporaryDamage = mDamage * 0.5f;

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

// キャラの現在ステータスを取得
const CharaStatus& CCharaBase::Status() const
{
	return mStatus;
}