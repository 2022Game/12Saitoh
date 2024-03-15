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

	// ダメージ処理
	virtual void TakeDamage(int damage);

	/// <summary>
	/// 攻撃開始
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// 攻撃終了
	/// </summary>
	virtual void AttackEnd();

private:
	int mDamage; // ダメージ値

protected:
	/// <summary>
	/// 攻撃がヒットしたオブジェクトを追加
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// 既に攻撃がヒットしているオブジェクトかどうか
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	std::list<CObjectBase*> mAttackHitObjects;

	CharaStatus mMaxStatus;		// キャラの最大ステータス
	CharaStatus mStatus;		// キャラの現在のステータス

	int mTemporaryDamage;	// 暫定ダメージ
};
