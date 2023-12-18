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

	/// <summary>
	/// プレイヤーが敵に与えるダメージ
	/// </summary>
	/// <param name="atk">プレイヤーの攻撃力</param>
	/// <param name="def">敵の防御力</param>
	/// <param name="motionvalue">モーション値</param>
	/// <param name="multiplier">肉質倍率</param>
	/// <returns>ダメージ</returns>
	int TakeEnemyToDamage(const int atk, const int def, const float motionvalue, const float multiplier);
	/// <summary>
	/// 敵がプレイヤーに与えるダメージ
	/// </summary>
	/// <param name="atk">敵の攻撃力</param>
	/// <param name="def">プレイヤーの防御力</param>
	/// <param name="motionvalue">モーション値</param>
	/// <returns>被ダメージ値</returns>
	int TakePlayerToDamage(const int atk, const int def, const float motionvalue);

	// ダメージ軽減率の計算
	float DamageReductionRate(int def);

	// ダメージ値の取得
	int GetDamage() const;
	// 暫定ダメージ値の取得
	int GetTemporaryDamage() const;

	// キャラの現在ステータスを取得
	const CharaStatus& Status() const;

	virtual void TakeDamage(int damage);

private:
	int mDamage; // ダメージ値

protected:
	CharaStatus mStatus;	// キャラクターステータス
	int mTemporaryDamage;	// 暫定ダメージ
};
