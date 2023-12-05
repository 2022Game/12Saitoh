#pragma once
#include "CObjectBase.h"
#include "CharaStatus.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	/* ダメージ計算 */
	// プレイヤーが敵に与えるダメージ計算
	int TakeEnemyToDamage();
	// 敵がプレイヤーに与えるダメージ計算
	int TakePlayerToDamage(const int atk, const int def, const float motionvalue);

	// ダメージ軽減率の計算
	float DamageReductionRate(int def);

	// ダメージ値の取得
	int GetDamage() const;
	// 暫定ダメージ値の取得
	int GetTemporaryDamage() const;

	// キャラの現在ステータスを取得
	const CharaStatus& Status() const;

private:
	int mDamage; // ダメージ値
protected:
	CharaStatus mStatus;	// キャラクターステータス
	int mTemporaryDamage;	// 暫定ダメージ
};
